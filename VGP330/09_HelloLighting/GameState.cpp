#include "GameState.h"

using namespace WallG::Graphics;
using namespace WallG::Input;
using namespace WallG::Math;


void GameState::Initialize()
{
    GraphicsSystem::Get()->SetClearColor(Colors::DimGray);

    // -------------------------------------------
    // Create vertex buffer
    mSphere = MeshBuilder::CreatCubePN();
    mMeshBuffer.Initialize(mSphere);
    //Set material
    mMaterial.ambient = {1};
    mMaterial.specular = {0.5};
    mMaterial.diffuse = {0.5};
    mMaterial.Power = {10.0f};
    
    mDirctionalLight.ambient = { 1 };
    mDirctionalLight.diffuse = { 0.5 };
    mDirctionalLight.specular = { 0.5 };
    mDirctionalLight.direction = { 0.0f, 0 , 1 };

    const wchar_t* shaderFileNames[] = 
    {
        L"../../Assets/Shaders/DoGrouaudShading.fx",
        L"../../Assets/Shaders/DoPhongShading.fx"
    };

    for (size_t i = 0; i < std::size(shaderFileNames); ++i)
    {
        mVertexShader[i].Initialize(shaderFileNames[i], VertexPN::Format);
        mPixelShader[i].Initialize(shaderFileNames[i]);
    }
    // Creat constant buffer
    mConstantBuffer.Initialize();
    mLightBuffer.Initialize();
    mMaterialBuffer.Initialize();
}

void GameState::Terminate()
{
    mMaterialBuffer.Terminate();
    mLightBuffer.Terminate();
    mConstantBuffer.Terminate();
    mMeshBuffer.Terminate();
    for (auto& vs : mVertexShader)
        vs.Terminate();
    for (auto& ps : mPixelShader)
        ps.Terminate();
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
    

    Matrix4 matView = mCamera.GetViewMatrix();
    Matrix4 matProj = mCamera.GetProjectionMatrix();

    TransformData transformData;
    // viewPosition is camera's postion 
    transformData.viewPostion = mCamera.GetPosition();
    mConstantBuffer.BindVS(0);
    mConstantBuffer.BindPS(0);

    mLightBuffer.Update(mDirctionalLight);
    mLightBuffer.BindVS(1);
    mLightBuffer.BindPS(1);

    mMaterialBuffer.Update(mMaterial);
    mMaterialBuffer.BindVS(2);
    mMaterialBuffer.BindPS(2);

    const float xOffsets[] = { -2.0f, 2.0f };

    for (size_t i = 0; i < std::size(xOffsets); ++i)
    {
        Matrix4 matWorld = Matrix4::RotationX(mRotation.x) * Matrix4::RotationY(mRotation.y) * Matrix4::Translation({xOffsets[i],0.0f, 0.0f});
        transformData.world = Transpose(matWorld);
        transformData.wvp = Transpose(matWorld * matView * matProj);
        mConstantBuffer.Update(transformData);

        mVertexShader[i].Bind();
        mPixelShader[i].Bind();
        mMeshBuffer.Render();
    }


}

void GameState::DebugUI()
{
    ImGui::Begin("Debug Info", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
    ImGui::Text("fps: %f", mFPS);
    ImGui::DragFloat3("Rotation", (float*)&mRotation, 0.01f);

    ImGui::ColorEdit4("Material Ambinet", &mMaterial.ambient.x);
    ImGui::ColorEdit4("Material diffuse", &mMaterial.diffuse.x);
    ImGui::ColorEdit4("Material specular", &mMaterial.specular.x);
    ImGui::DragFloat ("Material Power", &mMaterial.Power,1.0f,1.0f,1000.0f);

    ImGui::ColorEdit4("DirctionalLight Ambinet", &mDirctionalLight.ambient.x);
    ImGui::ColorEdit4("DirctionalLight diffuse", &mDirctionalLight.diffuse.x);
    ImGui::ColorEdit4("DirctionalLight specular", &mDirctionalLight.specular.x);
    ImGui::DragFloat3("DirctionalLight direction", &mDirctionalLight.direction.x,0.1f);
    
    ImGui::End();
}
