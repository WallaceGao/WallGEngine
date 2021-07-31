#include "GameState.h"

using namespace WallG::Graphics;
using namespace WallG::Input;
using namespace WallG::Math;


void GameState::Initialize()
{
    // TODO:
    // - Create data for vertex buffer and index buffer for a cube
    //
    //
    // Cube
    //         5  +-----------------+  6
    //           /|                /|
    // 1 Y 1.0  +-----------------+ |2
    //          | |               | |
    //          | |               | |
    //          | |               | |
    //       4  | +---------------|-+ 1.0 Z 7
    //          |/                |/
    //  0 -1.0  +-----------------+ -1.0  3
    //      -1.0               1.0 X
    //
    //
    //      i.e.
    //        std::vector<PCEngine::Graphics::VertexPC> vertices;
    //        std::vector<uint32_t> indices;
    //
    // - Initialize MeshBuffer using the new Initialize overload


    mMesh = MeshBuilder::CreatCylinderPX(2,12, 4.0f, 1.0);

    // -------------------------------------------
    // Create vertex buffer
    mMeshBuffer.Initialize(mMesh);
    mVertexShader.Initialize(L"../../Assets/Shaders/DoTexturing.fx", VertexPX::Format);
    mPixelShader.Initialize(L"../../Assets/Shaders/DoTexturing.fx");
    mSampler.Initialize(Sampler::Filter::Linear, Sampler::AddressMode::Wrap);
    mTexture.Initialize("../../Assets/Images/Assignment3.jpg");
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

    // Set matrix then draw cube
    transformData.wvp = Transpose(matWorld * matView * matProj);
    mConstantBuffer.Update(transformData);
    mMeshBuffer.Render();

    // Update metrix then draw a seconed cube
    matWorld = Matrix4::RotationX(mRotation.x) * Matrix4::RotationY(mRotation.y) * Matrix4::Translation({ 5.0f,0.0f,0.0f });
    transformData.wvp = Transpose(matWorld * matView * matProj);
    mConstantBuffer.Update(transformData);
    mMeshBuffer.Render();

    // Update metrix then draw a thrid cube
    matWorld = Matrix4::RotationX(mRotation.x) * Matrix4::RotationY(mRotation.y) * Matrix4::Translation({ -5.0f,0.0f,0.0f });
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
