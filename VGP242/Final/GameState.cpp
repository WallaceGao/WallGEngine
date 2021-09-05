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
    

    mSkyboxMesh = MeshBuilder::CreatSkyboxPX(500.0f);
    mSphereMesh = MeshBuilder::CreatSpherePX(16,16,1);
    mSphereBuffer.Initialize(mSphereMesh);
    mPlanetScale.push_back(5);
    mPlanetScale.push_back(1);
    mPlanetScale.push_back(1.5);
    mPlanetScale.push_back(1.7);
    mPlanetScale.push_back(2);
    mPlanetScale.push_back(4);
    mPlanetScale.push_back(0.8);

    // -------------------------------------------
    // Create vertex buffer
    mVertexShader.Initialize(L"../../Assets/Shaders/DoTexturing.fx", VertexPX::Format);
    mPixelShader.Initialize(L"../../Assets/Shaders/DoTexturing.fx");

    mSampler.Initialize(Sampler::Filter::Linear, Sampler::AddressMode::Wrap);

    mMoonTexture.Initialize("../../Assets/Images/SolarTextures/2k_moon.jpg");
    mJupiterTexture.Initialize("../../Assets/Images/SolarTextures/2k_jupiter.jpg");
    mMarsTexture.Initialize("../../Assets/Images/SolarTextures/2k_mars.jpg");
    mVenusTexture.Initialize("../../Assets/Images/SolarTextures/2k_venus_surface.jpg");
    mEarthTexture.Initialize("../../Assets/Images/SolarTextures/2k_earth_daymap.jpg");
    mMercuryTexture.Initialize("../../Assets/Images/SolarTextures/2k_mercury.jpg");
    mSunTexture.Initialize("../../Assets/Images/SolarTextures/2k_sun.jpg");
    mSkyboxTexture.Initialize("../../Assets/Images/cubemap.png");
    mSkyboxMeshBuffer.Initialize(mSkyboxMesh);

    //Speed
    mMercurySpeed = 0.0f;
    mSelfMercurySpeed = 0.0f;
    mVenusSpeed = 0.0f;
    mSelfVenusSpeed = 0.0f;
    mEarthSpeed = 0.0f;
    mSelfEarthSpeed = 0.0f;
    mMarsSpeed = 0.0f;
    mSelfMarsSpeed = 0.0f;
    mJupiterSpeed = 1.5f;
    mSelfJupiterSpeed = 1.0f;

    // Creat constant buffer
    mConstantBuffer.Initialize();
}

void GameState::Terminate()
{
    mConstantBuffer.Terminate();
    mVertexShader.Terminate();
    mPixelShader.Terminate();
    mSampler.Terminate();
    
    mSphereBuffer.Terminate();
    //Jupiter
    mJupiterTexture.Terminate();

    //Mars
    mMarsTexture.Terminate();

    //Venus
    mVenusTexture.Terminate();

    //Earth
    mEarthTexture.Terminate();
    mMoonTexture.Terminate();
    //Mercury
    mMercuryTexture.Terminate();

    //Sun
    mSunTexture.Terminate();

    //Skybox
    mSkyboxTexture.Terminate();
    mSkyboxMeshBuffer.Terminate();
}

void GameState::Update(float deltaTime)
{
    mFPS = 1.0f / deltaTime;

    auto inputSystem = InputSystem::Get();


    //mSumRotation.y += deltaTime;

    mSumRotation.y -= deltaTime;
    //
    mMercuryRotation -= deltaTime * mMercurySpeed;
    mSelfMercuryRotation -= deltaTime * mSelfMercurySpeed;
    //
    mVenusRotation -= deltaTime * mVenusSpeed;
    mSelfVenusRotation -= deltaTime * mSelfVenusSpeed;
    //
    mEarthRotation -= deltaTime * mEarthSpeed;
    mSelfEarthRotation -= deltaTime * mSelfEarthSpeed;
    //
    mMarsRotation -= deltaTime * mMarsSpeed;
    mSelfMarsRotation -= deltaTime * mSelfMarsSpeed;
    //
    mJupiterRotation -= deltaTime * mJupiterSpeed;
    mSelfJupiterRotation -= deltaTime  * mSelfJupiterSpeed;


    //if input LSHIFT * 10, else 1.0f
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
    mSampler.BindPS(0);
    mVertexShader.Bind();
    mPixelShader.Bind();

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
    matWorld = Matrix4::RotationY(mSumRotation.y) * Matrix4::RotationY(mSumRotation.y) * Matrix4::Translation({ 0.0f,0.0f,0.0f });
    auto scale = Matrix4::Scaling(mPlanetScale[0]);
    transformData.wvp = Transpose(scale* matWorld * matView * matProj);
    mConstantBuffer.Update(transformData);
    mSphereBuffer.Render();

    // Mercury
    mMercuryTexture.BindPS(0);
    matWorld = Matrix4::RotationY(mSelfMercuryRotation.y) * Matrix4::Translation({ 15.0f,0.0f,0.0f }) * Matrix4::RotationY(mMercuryRotation.y);
    scale = Matrix4::Scaling(mPlanetScale[1]);
    transformData.wvp = Transpose(scale * matWorld * matView * matProj);
    mConstantBuffer.Update(transformData);
    mSphereBuffer.Render();

    // Venus
    mVenusTexture.BindPS(0);
    matWorld = Matrix4::RotationY(mSelfVenusRotation.y) * Matrix4::Translation({ 30.0f,0.0f,0.0f }) * Matrix4::RotationY(mVenusRotation.y);
    scale = Matrix4::Scaling(mPlanetScale[2]);
    transformData.wvp = Transpose(scale * matWorld * matView * matProj);
    mConstantBuffer.Update(transformData);
    mSphereBuffer.Render();

    // Earth
    mEarthTexture.BindPS(0);
    matWorld = Matrix4::RotationY(mSelfEarthRotation.y) * Matrix4::Translation({ 45.0f,0.0f,0.0f }) * Matrix4::RotationY(mEarthRotation.y);
    scale = Matrix4::Scaling(mPlanetScale[3]);
    transformData.wvp = Transpose(scale * matWorld * matView * matProj);
    mConstantBuffer.Update(transformData);
    mSphereBuffer.Render();

    // Moon
    mMoonTexture.BindPS(0);
    //
    auto matMoon = Matrix4::RotationY(mSelfMoonRotation.y) * Matrix4::Translation({ 5.0f,0.0f,0.0f }) * Matrix4::RotationY(mMoonRotation.y);
    scale = Matrix4::Scaling(mPlanetScale[6]);
    // mat world come from earth and we need keep it
    transformData.wvp = Transpose(scale * matMoon *  matWorld * matView * matProj);
    mConstantBuffer.Update(transformData);
    mSphereBuffer.Render();

    // Mars
    mMarsTexture.BindPS(0);
    matWorld = Matrix4::RotationY(mSelfMarsRotation.y) * Matrix4::Translation({ 60.0f,0.0f,0.0f }) * Matrix4::RotationY(mMarsRotation.y);
    scale = Matrix4::Scaling(mPlanetScale[4]);
    transformData.wvp = Transpose(scale * matWorld * matView * matProj);
    mConstantBuffer.Update(transformData);
    mSphereBuffer.Render();

    // Jupiter
    mJupiterTexture.BindPS(0);
    matWorld = Matrix4::RotationY(mSelfJupiterRotation.y) * Matrix4::Translation({ 75.0f,0.0f,0.0f }) * Matrix4::RotationY(mJupiterRotation.y);
    scale = Matrix4::Scaling(mPlanetScale[5]);
    transformData.wvp = Transpose(scale * matWorld * matView * matProj);
    mConstantBuffer.Update(transformData);
    mSphereBuffer.Render();

    //SimpleDraw::AddAABB({ 0,0,0 }, {15,15,15}, Colors::Aqua);
    //SimpleDraw::AddSphere({ 1,1,1 }, 16, Colors::Aqua, 32, 32);
    SimpleDraw::Render(mCamera);
}

void GameState::DebugUI()
{
    ImGui::Begin("Debug Info", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
    ImGui::Text("fps: %f", mFPS);

    ImGui::DragFloat("Mercury Rota Speed", (float*)&mMercurySpeed, 0.01f);
    ImGui::DragFloat("Mercury Self Rota Speed", (float*)&mSelfMercurySpeed, 0.01f);
    
    ImGui::DragFloat("Venus Rota Speed", (float*)&mVenusSpeed, 0.01f);
    ImGui::DragFloat("Venus Self Rota Speed", (float*)&mSelfVenusSpeed, 0.01f);
    
    ImGui::DragFloat("Earth Rota Speed", (float*)&mEarthSpeed, 0.01f);
    ImGui::DragFloat("Earth Self Rota Speed", (float*)&mSelfEarthSpeed, 0.01f);

    ImGui::DragFloat("Moon Rota Speed", (float*)&mMoonSpeed, 0.01f);
    ImGui::DragFloat("Moon Self Rota Speed", (float*)&mSelfMoonSpeed, 0.01f);
    
    ImGui::DragFloat("Mars Rota Speed", (float*)&mMarsSpeed, 0.01f);
    ImGui::DragFloat("Mars Self Rota Speed", (float*)&mSelfMarsSpeed, 0.01f);
    
    ImGui::DragFloat("Jupiter Rota Speed", (float*)&mJupiterSpeed, 0.01f);
    ImGui::DragFloat("Jupiter Self Rota Speed", (float*)&mSelfJupiterSpeed, 0.01f);

    

    ImGui::End();
}
