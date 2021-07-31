#include "GameState.h"

using namespace WallG::Graphics;
using namespace WallG::Input;
using namespace WallG::Math;

namespace
{
    Matrix4 GetProjectionMatrix()
    {
        const float h = 1.0f / tan(60.0f * 3.14f / 180.0f * 0.5f);
        const float w = h / GraphicsSystem::Get()->GetBackBufferAspectRatio();
        const float zf = 100.0f;
        const float zn = 0.1f;
        const float q = zf / (zf - zn);
        return {
            w,    0.0f, 0.0f,    0.0f,
            0.0f, h,    0.0f,    0.0f,
            0.0f, 0.0f, q,       1.0f,
            0.0f, 0.0f, -zn * q, 0.0f
        };
    }
}

void GameState::Initialize()
{
    // TODO:
    // - Create data for vertex buffer and index buffer for a cube
    //
    //
    // Cube
    //         6  +-----------------+  7
    //           /|                /|
    // 2 Y 1.0  +-----------------+ |3
    //          | |               | |
    //          | |               | |
    //          | |               | |
    //       4  | +---------------|-+ 1.0 Z 5
    //          |/                |/
    //  0 -1.0  +-----------------+ -1.0  1
    //      -1.0               1.0 X
    //
    //
    //      i.e.
    //        std::vector<PCEngine::Graphics::VertexPC> vertices;
    //        std::vector<uint32_t> indices;
    //
    // - Initialize MeshBuffer using the new Initialize overload




    
    // -------------------------------------------
    // Create vertex buffer
    mMesh = MeshBuilder::CreatCubePC();
    mMeshBuffer.Initialize(mMesh);

    // -------------------------------------------
    // Compile and create vertex shader
    mVertexShader.Initialize(L"../../Assets/Shaders/DoTransform.fx", VertexPC::Format);

    // -------------------------------------------
    // Compile and create pixel shader
    mPixelShader.Initialize(L"../../Assets/Shaders/DoTransform.fx");
    // -------------------------------------------

    // Creat constant buffer
    mConstantBuffer.Initialize();
    mPosition.z = 5.0f;
}

void GameState::Terminate()
{
    mConstantBuffer.Terminate();
    mMeshBuffer.Terminate();
    mVertexShader.Terminate();
    mPixelShader.Terminate();
}

void GameState::Update(float deltaTime)
{
    auto inputSystem = InputSystem::Get();

    //if (inputSystem->IsKeyDown(KeyCode::LEFT))
    //{
    //    mRotation.y += deltaTime;
    //}
    //if (inputSystem->IsKeyDown(KeyCode::RIGHT))
    //{
    //    mRotation.y -= deltaTime;
    //}

    //if (inputSystem->IsKeyDown(KeyCode::UP))
    //{
    //    mRotation.x += deltaTime;
    //}
    //if (inputSystem->IsKeyDown(KeyCode::DOWN))
    //{
    //    mRotation.x -= deltaTime;
    //}

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

    Matrix4 matWorld = Matrix4::RotationX(mRotation.x) * Matrix4::RotationY(mRotation.y)* Matrix4::Translation(mPosition);
    Matrix4 matView = Matrix4::Indentity;
    Matrix4 matProj = GetProjectionMatrix();
    TransformData transformData;

    // Set matrix then draw cube
    transformData.wvp = Transpose(matWorld * matView * matProj);
    mConstantBuffer.Update(transformData);
    mMeshBuffer.Render();

    // Update metrix then draw a seconed cube
    matWorld = Matrix4::RotationX(mRotation.x) * Matrix4::RotationY(mRotation.y) * Matrix4::Translation({ 2.0f,0.0f,0.0f });
    transformData.wvp = Transpose(matWorld * matView * matProj);
    mConstantBuffer.Update(transformData);
    mMeshBuffer.Render();
}
