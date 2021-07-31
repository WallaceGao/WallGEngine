#include "GameState.h"

using namespace WallG::Graphics;
using namespace WallG::Input;
using namespace WallG::Math;


void GameState::Initialize()
{
    
    ModelLoader::LoadObjPX("../../Assets/Model/trident_a10/trident_a10.obj", 0.1f , mMesh);


    // -------------------------------------------
    // Create vertex buffer
    mMeshBuffer.Initialize(mMesh);
    mVertexShader.Initialize(L"../../Assets/Shaders/DoTexturing.fx", VertexPX::Format);
    mPixelShader.Initialize(L"../../Assets/Shaders/DoTexturing.fx");
    mSampler.Initialize(Sampler::Filter::Linear, Sampler::AddressMode::Wrap);
    mTexture.Initialize("../../Assets/Model/trident_a10/trident_no_decal_diffuse.tif");
    // Creat constant buffer
    mConstantBuffer.Initialize();
}

void GameState::Terminate()
{
    mConstantBuffer.Terminate();
    mMeshBuffer.Terminate();
    mVertexShader.Terminate();
    mPixelShader.Terminate();
    mSampler.Terminate();
    mTexture.Terminate();
}

void GameState::Update(float deltaTime)
{
    mFPS = 1.0f / deltaTime;
    
    auto inputSystem = InputSystem::Get();

    const float moveSpeed = 10.0f * (inputSystem->IsKeyDown(KeyCode::LSHIFT) ? 10.0f : 1.0f);
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

    if (inputSystem->IsKeyDown(KeyCode::E))
    {
        //Quaternion quaternion 
        //mCamera.SetPosition();
    }
}

void GameState::Render()
{
    // Attach buffer to graphics pipeline
    mConstantBuffer.BindVS(0);
    mVertexShader.Bind();
    mPixelShader.Bind();
    
    mSampler.BindPS(0);
    mTexture.BindPS(0);

    Matrix4 matWorld = Matrix4::RotationX(mRotation.x) * Matrix4::RotationY(mRotation.y);
    Matrix4 matView = mCamera.GetViewMatrix();
    Matrix4 matProj = mCamera.GetProjectionMatrix();
    TransformData transformData;

    transformData.wvp = Transpose(matWorld * matView * matProj);
    mConstantBuffer.Update(transformData);
    mMeshBuffer.Render();

}

void GameState::DebugUI()
{
    ImGui::Begin("Debug Info", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
    ImGui::Text("fps: %f", mFPS);
    ImGui::DragFloat3("Rotation", (float*)&mRotation, 0.01f);
    ImGui::End();
}
