#include "GameState.h"

using namespace WallG::Graphics;
using namespace WallG::Input;
using namespace WallG::Math;


void GameState::Initialize()
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

    // character
    //mGreatSwordStrafe.Initialize("../../Assets/Model/GreatSword/GreatSwordSlash.model");
    //mGreatSwordStrafe.Initialize("../../Assets/Model/Monster/MutantPunch.model");
    //mGreatSwordStrafe.Initialize("../../Assets/Model/Keli/Keli.model");
    //mGreatSwordStrafe.Initialize("../../Assets/Model/Youyouzi/YouyouziStand.model");
    mGreatSwordStrafe.Initialize("../../Assets/Model/SpaceShip/SciFi_Fighter_AK5.model");
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
    //mAnimator.Initialize( &mGreatSwordStrafe);

}

void GameState::Terminate()
{
    mAnimator.Terminate();
    mSettingsBuffer.Terminate();
    mBoneTransformBuffer.Terminate();
    mConstantBuffer.Terminate();
    mMaterialBuffer.Terminate();
    mLightBuffer.Terminate();
    mGreatSwordStrafe.Terminate();
    mVertexShader.Terminate();
    mPixelShader.Terminate();
    mSampler.Terminate();
}

void GameState::Update(float deltaTime)
{
    mFPS = 1.0f / deltaTime;
    auto inputSystem = InputSystem::Get();

    if (inputSystem->IsKeyDown(KeyCode::LEFT))
    {
        mRotation.y += deltaTime;
    }
    if (inputSystem->IsKeyDown(KeyCode::RIGHT))
    {
        mRotation.y -= deltaTime;
    }

    if (inputSystem->IsKeyDown(KeyCode::UP))
    {
        mRotation.x += deltaTime;
    }
    if (inputSystem->IsKeyDown(KeyCode::DOWN))
    {
        mRotation.x -= deltaTime;
    }

    const float moveSpeed = 10.0f;
    const float turnSpeed = 10.0f * Constants::DegToRad;
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

    if (mShowAnimation)
    {
        mAnimator.SetLoop(mIsLoop);
        mAnimator.Update(deltaTime);
    }
}

void GameState::Render()
{
    // Attach buffer to graphics pipeline
    Matrix4 matView = mCamera.GetViewMatrix();
    Matrix4 matProj = mCamera.GetProjectionMatrix();

    // viewPosition is camera's postion 
    TransformData transformData;
    transformData.viewPostion = mCamera.GetPosition();

    //Draw Skybox
    Matrix4 matWorld = Matrix4::RotationX(mRotation.x) * Matrix4::RotationY(mRotation.y) * Matrix4::RotationZ(mRotation.z);

    transformData.wvp = Transpose(matWorld * matView * matProj);
    mConstantBuffer.BindVS(0);
    mConstantBuffer.BindPS(0);
    mConstantBuffer.Update(transformData);

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

    mSettings.useSkinning = mShowAnimation ? 1 : 0;
    mSettingsBuffer.Update(mSettings);
    mSettingsBuffer.BindVS(3);
    mSettingsBuffer.BindPS(3);

    if (mShowSkelton)
    {
        //mAnimator.Re(mShowAnimation,mShowSkelton);
        auto toLocal = mAnimator.GetToLocalTransforms();
        DrawSkeleton(*(mGreatSwordStrafe.skeleton), toLocal);
    }
    else if (mShowAnimation)
    {
        //mAnimator.Render(mShowAnimation,mShowSkelton);
        auto toLocal = mAnimator.GetToLocalTransforms();
        StandardBoneTransformData boneData;
        for (auto& bone : mGreatSwordStrafe.skeleton->bones)
        {
            boneData.boneTransform[bone->index] = Transpose(bone->offsetTransform * toLocal[bone->index]);
        }
        mBoneTransformBuffer.Update(boneData);
        mBoneTransformBuffer.BindVS(4);
        mGreatSwordStrafe.Render();
    }
    else
    {
        mGreatSwordStrafe.Render();
    }

    SimpleDraw::AddLine(Vector3::Zero, Vector3::XAxis, Colors::Red);
    SimpleDraw::AddLine(Vector3::Zero, Vector3::YAxis, Colors::Green);
    SimpleDraw::AddLine(Vector3::Zero, Vector3::ZAxis, Colors::Blue);
    SimpleDraw::Render(mCamera);
}

void GameState::DebugUI()
{
    ImGui::Begin("Debug Info", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
    ImGui::Text("fps: %f", mFPS);
    ImGui::DragFloat3("Rotation", (float*)&mRotation, 0.01f);

    ImGui::ColorEdit4("Material Ambinet", &mMaterial.ambient.x);
    ImGui::ColorEdit4("Material diffuse", &mMaterial.diffuse.x);
    ImGui::ColorEdit4("Material specular", &mMaterial.specular.x);
    ImGui::DragFloat("Material Power", &mMaterial.Power, 1.0f, 1.0f, 1000.0f);

    ImGui::ColorEdit4("DirctionalLight Ambinet", &mDirctionalLight.ambient.x);
    ImGui::ColorEdit4("DirctionalLight diffuse", &mDirctionalLight.diffuse.x);
    ImGui::ColorEdit4("DirctionalLight specular", &mDirctionalLight.specular.x);
    ImGui::DragFloat3("DirctionalLight direction", &mDirctionalLight.direction.x, 0.1f);

    ImGui::Checkbox("Show Skeleton", &mShowSkelton);
    ImGui::Checkbox("Show Animation", &mShowAnimation);
    ImGui::Checkbox("Loop Animation", &mIsLoop);

    ImGui::End();
}
