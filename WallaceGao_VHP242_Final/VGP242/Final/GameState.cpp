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



    mCamera.SetPosition({ 0.0f,0.0f,-100.0f });

    mJupiterMesh = MeshBuilder::CreatSphere(3, 4, 4);
    mMarsMesh = MeshBuilder::CreatSphere(16, 16, 2);
    mVenusMesh = MeshBuilder::CreatSphere(16, 16, 3);
    mEarthMesh = MeshBuilder::CreatSphere(16, 16, 1);
    mMercuryMesh = MeshBuilder::CreatSphere(16, 16, 0.5);
    mSunMesh = MeshBuilder::CreatSphere(16, 16, 5);
    mSkyboxMesh = MeshBuilder::CreatSkybox(500.0f);



    // -------------------------------------------
    // Create vertex buffer
    mVertexShader.Initialize(L"../../Assets/Shaders/DoTexturing.fx", VertexPX::Format);
    mPixelShader.Initialize(L"../../Assets/Shaders/DoTexturing.fx");
    mSampler.Initialize(Sampler::Filter::Linear, Sampler::AddressMode::Wrap);

    
    mJupiterTexture.Initialize("../../Assets/Images/8k_jupiter.jpg");
    mJupiterMeshBuffer.Initialize(mJupiterMesh);

    mMarsTexture.Initialize("../../Assets/Images/8k_mars.jpg");
    mMarsMeshBuffer.Initialize(mMarsMesh);

    mVenusTexture.Initialize("../../Assets/Images/8k_venus_surface.jpg");
    mVenusMeshBuffer.Initialize(mVenusMesh);

    mEarthTexture.Initialize("../../Assets/Images/8k_earth_daymap.jpg");
    mEarthMeshBuffer.Initialize(mEarthMesh);

    mMercuryTexture.Initialize("../../Assets/Images/8k_mercury.jpg");
    mMercuryMeshBuffer.Initialize(mMercuryMesh);

    mSunTexture.Initialize("../../Assets/Images/8k_sun.jpg");
    mSunMeshBuffer.Initialize(mSunMesh);

    mSkyboxTexture.Initialize("../../Assets/Images/cubemap.png");
    mSkyboxMeshBuffer.Initialize(mSkyboxMesh);

    //Speed
    mMercurySpeed = 0.5f;

    mVenusSpeed = 2.0f;

    mEarthSpeed = 0.0f;

    mMarsSpeed = 1.0f;

    mJupiterSpeed = -1.0f;

    // Creat constant buffer
    mConstantBuffer.Initialize();
}

void GameState::Terminate()
{
    mConstantBuffer.Terminate();
    mVertexShader.Terminate();
    mPixelShader.Terminate();
    mSampler.Terminate();
    
    //Jupiter
    mJupiterMeshBuffer.Terminate();
    mJupiterTexture.Terminate();

    //Mars
    mMarsMeshBuffer.Terminate();
    mMarsTexture.Terminate();

    //Venus
    mVenusMeshBuffer.Terminate();
    mVenusTexture.Terminate();

    //Earth
    mEarthMeshBuffer.Terminate();
    mEarthTexture.Terminate();

    //Mercury
    mMercuryMeshBuffer.Terminate();
    mMercuryTexture.Terminate();

    //Sun
    mSunMeshBuffer.Terminate();
    mSunTexture.Terminate();

    //Skybox
    mSkyboxTexture.Terminate();
    mSkyboxMeshBuffer.Terminate();
}

void GameState::Update(float deltaTime)
{
    mFPS = 1.0f / deltaTime;

    auto inputSystem = InputSystem::Get();


        //mRotation.y += deltaTime;

        mRotation.y -= deltaTime;

    if (inputSystem->IsKeyDown(KeyCode::UP))
    {
        mRotation.x += deltaTime;
    }
    if (inputSystem->IsKeyDown(KeyCode::DOWN))
    {
        mRotation.x -= deltaTime;
    }

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


}

void GameState::Render()
{
    // Attach buffer to graphics pipeline
    mConstantBuffer.BindVS(0);
    mVertexShader.Bind();
    mPixelShader.Bind();
    mSampler.BindPS(0);

    Matrix4 matWorld = Matrix4::Translation(mCamera.GetPosition());
    Matrix4 matView = mCamera.GetViewMatrix();
    Matrix4 matProj = mCamera.GetProjectionMatrix();
    TransformData transformData;
    transformData.wvp = Transpose(matWorld * matView * matProj);
    mConstantBuffer.Update(transformData);

    mSkyboxTexture.BindPS(0);
    mSkyboxMeshBuffer.Render();

    //// Sun
    mSunTexture.BindPS(0);
    matWorld = Matrix4::RotationX(mRotation.x) * Matrix4::RotationY(mRotation.y) * Matrix4::Translation({ 0.0f,0.0f,0.0f });
    transformData.wvp = Transpose(matWorld * matView * matProj);
    mConstantBuffer.Update(transformData);
    mSunMeshBuffer.Render();

    // Mercury
    mMercuryTexture.BindPS(0);
    matWorld = Matrix4::RotationY(mRotation.y + 5.0f) * Matrix4::Translation({ 15.0f,0.0f,0.0f }) * Matrix4::RotationY(mRotation.y + mMercurySpeed);
    transformData.wvp = Transpose(matWorld * matView * matProj);
    mConstantBuffer.Update(transformData);
    mMercuryMeshBuffer.Render();

    // Venus
    mVenusTexture.BindPS(0);
    matWorld = Matrix4::RotationY(mRotation.y + 5.0f) * Matrix4::Translation({ 30.0f,0.0f,0.0f }) * Matrix4::RotationY(mRotation.y + mVenusSpeed);
    transformData.wvp = Transpose(matWorld * matView * matProj);
    mConstantBuffer.Update(transformData);
    mVenusMeshBuffer.Render();

    // Earth
    mEarthTexture.BindPS(0);
    matWorld = Matrix4::RotationY(mRotation.y + 5.0f) * Matrix4::Translation({ 45.0f,0.0f,0.0f }) * Matrix4::RotationY(mRotation.y + mEarthSpeed);
    transformData.wvp = Transpose(matWorld * matView * matProj);
    mConstantBuffer.Update(transformData);
    mEarthMeshBuffer.Render();

    // Mars
    mMarsTexture.BindPS(0);
    matWorld = Matrix4::RotationY(mRotation.y + 5.0f) * Matrix4::Translation({ 60.0f,0.0f,0.0f }) * Matrix4::RotationY(mRotation.y + mMarsSpeed);
    transformData.wvp = Transpose(matWorld * matView * matProj);
    mConstantBuffer.Update(transformData);
    mMarsMeshBuffer.Render();

    // Jupiter
    mJupiterTexture.BindPS(0);
    matWorld = Matrix4::RotationY(mRotation.y + 5.0f) * Matrix4::Translation({ 75.0f,0.0f,0.0f }) * Matrix4::RotationY(mRotation.y + mJupiterSpeed);
    transformData.wvp = Transpose(matWorld * matView * matProj);
    mConstantBuffer.Update(transformData);
    mJupiterMeshBuffer.Render();
}

void GameState::DebugUI()
{
    ImGui::Begin("Debug Info", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
    ImGui::Text("fps: %f", mFPS);
    ImGui::DragFloat("Mercury Speed", (float*)&mMercurySpeed, 0.01f);
    ImGui::DragFloat("Venus Speed", (float*)&mVenusSpeed, 0.01f);
    ImGui::DragFloat("Earth Speed", (float*)&mEarthSpeed, 0.01f);
    ImGui::DragFloat("Mars Speed", (float*)&mMarsSpeed, 0.01f);
    ImGui::DragFloat("Jupiter Speed", (float*)&mJupiterSpeed, 0.01f);
    ImGui::End();
}
