#include "Precompiled.h"
#include "RenderService.h"

#include "AnimatorComponent.h"
#include "CameraService.h"
#include "GameObject.h"
#include "GameWorld.h"
#include "modelComponent.h"
#include "TransformComponent.h"

using namespace WallG;
using namespace WallG::Graphics;
using namespace WallG::Input;
using namespace WallG::Math;

void RenderService::Initialize()
{
    GraphicsSystem::Get()->SetClearColor(Colors::DimGray);

    mMaterial.ambient = { 0.35f };
    mMaterial.specular = { 0.5f };
    mMaterial.diffuse = { 0.5f };
    mMaterial.Power = { 5.0f };

    mDirctionalLight.ambient = { 1.0f };
    mDirctionalLight.diffuse = { 0.5f };
    mDirctionalLight.specular = { 0.5f };
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
}

void RenderService::Terminate()
{
    mSettingsBuffer.Terminate();
    mBoneTransformBuffer.Terminate();
    mConstantBuffer.Terminate();
    mMaterialBuffer.Terminate();
    mLightBuffer.Terminate();
    mVertexShader.Terminate();
    mPixelShader.Terminate();
    mSampler.Terminate();
}

void RenderService::Render()
{
    // Attach buffer to graphics pipeline
    auto cameraService = GetWorld().GetService<CameraService>();
    Matrix4 matView = cameraService->GetCamera().GetViewMatrix();
    Matrix4 matProj = cameraService->GetCamera().GetProjectionMatrix();

    // viewPosition is camera's postion 
    TransformData transformData;
    transformData.viewPostion = cameraService->GetCamera().GetPosition();
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
        Quaternion localRotation = entry.modelComponent->GetRotation();

        Vector3 position = entry.transformComponent->GetPosition();
        Quaternion rotation = entry.transformComponent->GetRotation();
        Vector3 scale = entry.transformComponent->GetScale();

        Matrix4 matWorld =
            Matrix4::Scaling(scale) *
            Matrix4::RotationQuaternion(localRotation * rotation) *
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

    SimpleDraw::Render(cameraService->GetCamera());
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

