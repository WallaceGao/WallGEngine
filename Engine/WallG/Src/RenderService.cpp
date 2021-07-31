#include "Precompiled.h"
#include "RenderService.h"

#include "AnimatorComponent.h"
#include "GameObject.h"
#include "modelComponent.h"
#include "TransformComponent.h"

using namespace WallG;
using namespace WallG::Graphics;
using namespace WallG::Input;
using namespace WallG::Math;

void RenderService::Initialize()
{
    GraphicsSystem::Get()->SetClearColor(Colors::DimGray);

    mMaterial.ambient = { 1 };
    mMaterial.specular = { 0.5 };
    mMaterial.diffuse = { 0.5 };
    mMaterial.Power = { 10.0f };

    mDirctionalLight.ambient = { 1 };
    mDirctionalLight.diffuse = { 0.5 };
    mDirctionalLight.specular = { 0.5 };
    mDirctionalLight.direction = { 0.0f, 0 , 1 };

    //Set material
    mVertexShader.Initialize(L"../../Assets/Shaders/Standard.fx", BoneVertex::Format);
    mPixelShader.Initialize(L"../../Assets/Shaders/Standard.fx");

    // Creat constant buffer
    mSampler.Initialize(Sampler::Filter::Anisotropic, Sampler::AddressMode::Wrap);
    mLightBuffer.Initialize();
    mMaterialBuffer.Initialize();
    mConstantBuffer.Initialize();
    mBoneTransformBuffer.Initialize();
    mSettingsBuffer.Initialize();
    mCamera.SetPosition({ 0.0f, 0.0f, -3.0f });
}

void RenderService::Terminate()
{
    //mAnimator.Terminate();
    mSettingsBuffer.Terminate();
    mBoneTransformBuffer.Terminate();
    mConstantBuffer.Terminate();
    mMaterialBuffer.Terminate();
    mLightBuffer.Terminate();
    //mGreatSwordStrafe.Terminate();
    mVertexShader.Terminate();
    mPixelShader.Terminate();
    mSampler.Terminate();
}

void RenderService::Update(float deltaTime)
{
    const float moveSpeed = 10.0f;
    const float turnSpeed = 10.0f * Constants::DegToRad;

    auto inputSystem = InputSystem::Get();
    if (inputSystem->IsKeyDown(KeyCode::W))
        mCamera.Walk(moveSpeed * deltaTime);
    if (inputSystem->IsKeyDown(KeyCode::S))
        mCamera.Walk(-moveSpeed * deltaTime);
    if (inputSystem->IsKeyDown(KeyCode::D))
        mCamera.Strafe(moveSpeed * deltaTime);
    if (inputSystem->IsKeyDown(KeyCode::A))
        mCamera.Strafe(-moveSpeed * deltaTime);
    if (inputSystem->IsMouseDown(MouseButton::RBUTTON))
    {
        mCamera.Yaw(inputSystem->GetMouseMoveX() * turnSpeed * deltaTime);
        mCamera.Pitch(inputSystem->GetMouseMoveY() * turnSpeed * deltaTime);
    }
}

void WallG::RenderService::Render()
{
    // Attach buffer to graphics pipeline
    Matrix4 matView = mCamera.GetViewMatrix();
    Matrix4 matProj = mCamera.GetProjectionMatrix();

    // viewPosition is camera's postion 
    TransformData transformData;
    transformData.viewPostion = mCamera.GetPosition();
    mConstantBuffer.BindVS(0);
    mConstantBuffer.BindPS(0);

    mLightBuffer.Update(mDirctionalLight);
    mLightBuffer.BindVS(1);
    mLightBuffer.BindPS(1);

    mMaterialBuffer.Update(mMaterial);
    mMaterialBuffer.BindVS(2);
    mMaterialBuffer.BindPS(2);

    mSampler.BindVS(0);
    mSampler.BindPS(0);

    mVertexShader.Bind();
    mPixelShader.Bind();

    mSettingsBuffer.BindVS(3);
    mSettingsBuffer.BindPS(3);

    for (auto& entry : mRenderEntries)
    {
        Vector3 position = entry.transformComponent->GetPosition();
        Vector3 rotation = entry.transformComponent->GetRotation();

        Matrix4 matWorld =
            Matrix4::RotationX(rotation.x) *
            Matrix4::RotationY(rotation.y) *
            Matrix4::RotationZ(rotation.z) *
            Matrix4::Translation(position);
        transformData.wvp = Transpose(matWorld * matView * matProj);
        mConstantBuffer.Update(transformData);

        auto modelComponent = entry.modelComponent;
        auto& model = *modelComponent->GetModel();

        auto animatorComponent = entry.animatorComponent;
        if (animatorComponent)
        {
            auto& animator = animatorComponent->GetAnimator();

            StandardBoneTransformData boneData;
            const auto& tolocal = animator.GetToLocalTransforms();

            for (auto& bone : model.skeleton->bones)
            {
                boneData.boneTransform[bone->index] = Math::Transpose(tolocal[bone->index]);
            }
            mBoneTransformBuffer.Update(boneData);
            mBoneTransformBuffer.BindVS(4);

            mSettings.useSkinning = 1;
        }
        else
        {
            mSettings.useSkinning = 0;
        }


        for (size_t i = 0; i < model.meshData.size(); ++i)
        {
            const Model::MaterialData& material = model.materialData[model.meshData[i]->materiaIndex];
            if (material.diffuseMap)
                material.diffuseMap->BindPS(0);
            if (material.specularMap)
                material.specularMap->BindPS(1);
            if (material.normalMap)
                material.normalMap->BindPS(3);

            mSettings.specularMapWeight = material.specularMap ? (float)1 : (float)0;
            mSettings.normalMapWeight = material.normalMap ? (float)1 : (float)0;
            mSettingsBuffer.Update(mSettings);

            model.meshData[i]->meshBuffer.Render();
        }
    }

    SimpleDraw::Render(mCamera);
}

void RenderService::DebugUI()
{
    ImGui::ColorEdit4("Material Ambinet", &mMaterial.ambient.x);
    ImGui::ColorEdit4("Material diffuse", &mMaterial.diffuse.x);
    ImGui::ColorEdit4("Material specular", &mMaterial.specular.x);
    ImGui::DragFloat("Material Power", &mMaterial.Power, 1.0f, 1.0f, 1000.0f);

    ImGui::ColorEdit4("DirctionalLight Ambinet", &mDirctionalLight.ambient.x);
    ImGui::ColorEdit4("DirctionalLight diffuse", &mDirctionalLight.diffuse.x);
    ImGui::ColorEdit4("DirctionalLight specular", &mDirctionalLight.specular.x);
    ImGui::DragFloat3("DirctionalLight direction", &mDirctionalLight.direction.x, 0.1f);
}

void RenderService::Register(const ModelComponent* modelComponent)
{
    auto& gameObject = modelComponent->GetOwner();
    Entry& entry = mRenderEntries.emplace_back();
    entry.animatorComponent = gameObject.GetComponent<AnimatorComponent>();
    entry.modelComponent = modelComponent;
    entry.transformComponent = gameObject.GetComponent<TransformComponent>();
}

void RenderService::Unregister(const ModelComponent* modelComponent)
{
    auto match = [&](const auto& entry) {return (entry.modelComponent == modelComponent); };
    auto iter = std::find_if(mRenderEntries.begin(), mRenderEntries.end(), match);
    if (iter != mRenderEntries.end())
    {
        mRenderEntries.erase(iter);
    }
}

