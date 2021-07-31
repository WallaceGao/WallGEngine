#include "GameState.h"

using namespace WallG::Graphics;
using namespace WallG::Input;
using namespace WallG::Math;


void GameState::Initialize()
{
    GraphicsSystem::Get()->SetClearColor(Colors::DimGray);

    mCamera.SetPosition({ 0.0f, 0.0f, -3.0f });

    mRenderTarget.Initialize(
        GraphicsSystem::Get()->GetBackBufferWidth(),
        GraphicsSystem::Get()->GetBackBufferHeight()
    );

    mVertexShader.Initialize(L"../../Assets/Shaders/Earth.fx", Vertex::Format);
    mPixelShader.Initialize(L"../../Assets/Shaders/Earth.fx");
    mVertexSharderCloud.Initialize(L"../../Assets/Shaders/Earth.fx", Vertex::Format,"VSCloud");
    mPixelShaderCloud.Initialize(L"../../Assets/Shaders/Earth.fx", "PSCloud");
    mPostProcessThermalVertexShader.Initialize(L"../../Assets/Shaders/PostProcessThermal.fx", VertexPX::Format);
    mPostProcessThermalPixelShader.Initialize(L"../../Assets/Shaders/PostProcessThermal.fx");
    mPostProcessOldStyleVertexShader.Initialize(L"../../Assets/Shaders/PostProcess.fx", VertexPX::Format);
    mPostProcessOldStylePixelShader.Initialize(L"../../Assets/Shaders/PostProcess.fx");
    mPostProcessNightVisionVertexShader.Initialize(L"../../Assets/Shaders/PostProcessNightVision.fx", VertexPX::Format);
    mPostProcessNightVisionPixelShader.Initialize(L"../../Assets/Shaders/PostProcessNightVision.fx");
    mNoiseTexture.Initialize("../../Assets/Images/noise_texture_0001.png");


    //Earth texture
    mEarthTexture.Initialize("../../Assets/Images/EarthTextures/earth.jpg");
    mEarthDisplace.Initialize("../../Assets/Images/EarthTextures/earth_bump.jpg");
    mEarthSpec.Initialize("../../Assets/Images/EarthTextures/earth_spec.jpg");
    mEarthNormal.Initialize("../../Assets/Images/EarthTextures/earth_normal.jpg");
    mEarthClouds.Initialize("../../Assets/Images/EarthTextures/earth_clouds.jpg");
    mEarthNight.Initialize("../../Assets/Images/EarthTextures/earth_lights.jpg");

    //Skybox
    mSkyboxTexture.Initialize("../../Assets/Images/cubemap.png");
    mSkyboxMesh = MeshBuilder::CreatSkyboxPX(500.0f);
    mSkyboxMeshBuffer.Initialize(mSkyboxMesh);
    mConstantBufferSkyBox.Initialize();
    mVertexShaderSkyBox.Initialize(L"../../Assets/Shaders/DoTexturing.fx", VertexPX::Format);
    mPixelShaderSkyBox.Initialize(L"../../Assets/Shaders/DoTexturing.fx");

    mSampler.Initialize(Sampler::Filter::Anisotropic, Sampler::AddressMode::Wrap);

    // -------------------------------------------
    // Create vertex buffer
    mSphere = MeshBuilder::CreatSphere(256,256,1);
    mMeshBuffer.Initialize(mSphere);
    //Set material
    mMaterial.ambient = {0};
    mMaterial.specular = {1};
    mMaterial.diffuse = {1};
    mMaterial.Power = {10.0f};
    
    mBlendStateClouds.Initialize(BlendState::Mode::Additive);
    mDirctionalLight.ambient = { 0 };
    mDirctionalLight.diffuse = {1 };
    mDirctionalLight.specular = { 1 };
    mDirctionalLight.direction = { 0.0f, 0 , 1 };

    // Creat constant buffer
    mThermalBuffer.Initialize();
    mNightVisionBuffer.Initialize();
    mConstantBuffer.Initialize();
    mLightBuffer.Initialize();
    mMaterialBuffer.Initialize();
    mSettingsBuffer.Initialize();

    mScreenMash.vertices.push_back({ { -1.0f,-1.0f, 0.0f }, {0.0f,1.0f} });
    mScreenMash.vertices.push_back({ { -1.0f, 1.0f, 0.0f }, {0.0f,0.0f} });
    mScreenMash.vertices.push_back({ {  1.0f, 1.0f, 0.0f }, {1.0f,0.0f} });
    mScreenMash.vertices.push_back({ {  1.0f,-1.0f, 0.0f }, {1.0f,1.0f} });

    mScreenMash.indices.push_back(0);
    mScreenMash.indices.push_back(1);
    mScreenMash.indices.push_back(2);

    mScreenMash.indices.push_back(0);
    mScreenMash.indices.push_back(2);
    mScreenMash.indices.push_back(3);

    mScreenMeshBuffer.Initialize(mScreenMash);
}

void GameState::Terminate()
{
    mSettingsBuffer.Terminate();
    mMaterialBuffer.Terminate();
    mLightBuffer.Terminate();
    mConstantBuffer.Terminate();
    mMeshBuffer.Terminate();
    mVertexShader.Terminate();
    mPixelShader.Terminate();
    mVertexSharderCloud.Terminate();
    mPixelShaderCloud.Terminate();
    mSampler.Terminate();
    mBlendStateClouds.Terminate();
    mEarthNormal.Terminate();
    mEarthSpec.Terminate();
    mEarthDisplace.Terminate();
    mEarthTexture.Terminate();
    mEarthClouds.Terminate();
    mEarthNight.Terminate();
    mVertexShaderSkyBox.Terminate();
    mPixelShaderSkyBox.Terminate();
    mSkyboxTexture.Terminate();
    mSkyboxMeshBuffer.Terminate();
    mConstantBufferSkyBox.Terminate();
    mRenderTarget.Terminate();
    mScreenMeshBuffer.Terminate();
    mThermalBuffer.Terminate();

    mNightVisionBuffer.Terminate();
    mNoiseTexture.Terminate();
    mPostProcessOldStylePixelShader.Terminate();
    mPostProcessOldStyleVertexShader.Terminate();
    mPostProcessThermalPixelShader.Terminate();
    mPostProcessThermalVertexShader.Terminate();
    mPostProcessNightVisionVertexShader.Terminate();
    mPostProcessNightVisionPixelShader.Terminate();
}

void GameState::Update(float deltaTime)
{
    mFPS = 1.0f / deltaTime;
    mCloudRota -= deltaTime * mCloudRotaSpeed;
    mEarthRota -= deltaTime * mEarthRotaSpeed;
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
    // Capture a screen shot
    RenderScene();

    // Process screen shot before Pressenting 
    if (postProcess == 1)
    {
        PostProcessOldStyle();
    }
    else if(postProcess == 2)
    {
        PostProcessThermal();
    }
    else if(postProcess == 3)
    {
        PostProcessNightVision();
    }
}

void GameState::DebugUI()
{
    ImGui::Begin("Debug Info", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
    ImGui::Text("fps: %f", mFPS);
    ImGui::DragFloat3("Rotation", (float*)&mRotation, 0.01f);

    if (ImGui::CollapsingHeader("Material"))
    {
        ImGui::ColorEdit4("Material Ambinet", &mMaterial.ambient.x);
        ImGui::ColorEdit4("Material diffuse", &mMaterial.diffuse.x);
        ImGui::ColorEdit4("Material specular", &mMaterial.specular.x);
        ImGui::DragFloat ("Material Power", &mMaterial.Power,1.0f,1.0f,100.0f);
    }

    if (ImGui::CollapsingHeader("DirctionalLight"))
    {
        ImGui::ColorEdit4("DirctionalLight Ambinet", &mDirctionalLight.ambient.x);
        ImGui::ColorEdit4("DirctionalLight diffuse", &mDirctionalLight.diffuse.x);
        ImGui::ColorEdit4("DirctionalLight specular", &mDirctionalLight.specular.x);
        ImGui::DragFloat3("DirctionalLight direction", &mDirctionalLight.direction.x,0.1f,-2.0f,2.0f);
    }

    static bool normalMap = true;
    static bool specularMap = true;

    ImGui::DragFloat("BumpMap", &mSettings.bumpMapWeight, 0.1f, 0.0f, 1.0f);
    if (ImGui::Checkbox("NormalMap", &normalMap))
    {
        mSettings.normalMapWeight = normalMap ? 1.0f : 0.0f;
    }
    if (ImGui::Checkbox("SpecularMap", &specularMap))
    {
        mSettings.specularMapWeight = specularMap ? 1.0f : 0.0f;
    }

    ImGui::DragFloat("Cloud Rota Speed", (float*)&mCloudRotaSpeed, 0.01f);
    ImGui::DragFloat("Earth Rota Speed", (float*)&mEarthRotaSpeed, 0.01f);

    if (ImGui::RadioButton("no post process", &postProcess, 0))
    {
        postProcess = 0;
    }
    if (ImGui::RadioButton("Old Style", &postProcess, 1))
    {
        postProcess = 1;
    }
    if (ImGui::RadioButton("Thermal", &postProcess, 2))
    {
        postProcess = 2;
    }
    if (ImGui::RadioButton("Night Vision", &postProcess, 3))
    {
        postProcess = 3;
    }
    ImGui::End();
}

void GameState::RenderScene()
{
    if (postProcess != 0)
    {
        mRenderTarget.BeginRender();
    }
    // Attach buffer to graphics pipeline
    TransformData transformData;
    SkyBox skyBox;
    Matrix4 matView = mCamera.GetViewMatrix();
    Matrix4 matProj = mCamera.GetProjectionMatrix();

    // viewPosition is camera's postion 
    transformData.viewPostion = mCamera.GetPosition();

    //Draw Skybox
    Matrix4 matWorld = Matrix4::RotationX(mRotation.x) * Matrix4::RotationY(mRotation.y) * Matrix4::RotationZ(mRotation.z);
    skyBox.mvp = Transpose(matWorld * matView * matProj);
    mConstantBufferSkyBox.Update(skyBox);
    mConstantBufferSkyBox.BindVS(0);
    mVertexShaderSkyBox.Bind();
    mPixelShaderSkyBox.Bind();
    mSkyboxTexture.BindPS(0);
    mSkyboxMeshBuffer.Render();


    mConstantBuffer.BindVS(0);
    mConstantBuffer.BindPS(0);

    mLightBuffer.Update(mDirctionalLight);
    mLightBuffer.BindVS(1);
    mLightBuffer.BindPS(1);

    mMaterialBuffer.Update(mMaterial);
    mMaterialBuffer.BindVS(2);
    mMaterialBuffer.BindPS(2);

    mSettingsBuffer.Update(mSettings);
    mSettingsBuffer.BindVS(3);
    mSettingsBuffer.BindPS(3);

    mSampler.BindVS(0);
    mSampler.BindPS(0);

    mEarthDisplace.BindVS(2);
    mEarthSpec.BindPS(1);
    mEarthTexture.BindPS(0);
    mEarthNight.BindPS(5);
    mEarthNormal.BindPS(3);

    matWorld = Matrix4::RotationX(mRotation.x) * Matrix4::RotationY(mRotation.y + mEarthRota) * Matrix4::RotationZ(mRotation.z);
    transformData.wvp = Transpose(matWorld * matView * matProj);
    transformData.world = Transpose(matWorld);
    mConstantBuffer.Update(transformData);

    mVertexShader.Bind();
    mPixelShader.Bind();
    mMeshBuffer.Render();


    //Draw cloud
    mBlendStateClouds.Set();
    matWorld = Matrix4::Scaling(0.99) * Matrix4::RotationX(mRotation.x) * Matrix4::RotationY(mRotation.y + mCloudRota) * Matrix4::RotationZ(mRotation.z);
    transformData.world = Transpose(matWorld);
    transformData.wvp = Transpose(matWorld * matView * matProj);
    mConstantBuffer.Update(transformData);
    mEarthClouds.BindPS(4);
    mVertexSharderCloud.Bind();
    mPixelShaderCloud.Bind();
    mMeshBuffer.Render();
    mBlendStateClouds.ClearState();
    if (postProcess != 0)
    {
        mRenderTarget.EndRender();
    }
}

void GameState::PostProcessOldStyle()
{
    mPostProcessOldStylePixelShader.Bind();
    mPostProcessOldStyleVertexShader.Bind();

    mSampler.BindPS(0);

    mRenderTarget.BindPS(0);
    mScreenMeshBuffer.Render();
    mRenderTarget.UnbindPS(0);
}

void GameState::PostProcessThermal()
{

    mPostProcessThermalPixelShader.Bind();
    mPostProcessThermalVertexShader.Bind();

    ThermalBuffer buffer;
    buffer.vxOffset = 1;
    mThermalBuffer.Update(buffer);
    mThermalBuffer.BindPS(0);

    mSampler.BindPS(0);

    mRenderTarget.BindPS(0);
    mScreenMeshBuffer.Render();
    mRenderTarget.UnbindPS(0);
}

void GameState::PostProcessNightVision()
{
    mPostProcessNightVisionVertexShader.Bind();
    mPostProcessNightVisionPixelShader.Bind();

    NightVisionBuffer buffer;
    buffer.elapsedTime = 10.0f;
    buffer.colorAmplification = 4.0f;
    buffer.luminanceThreshold = 0.2f;
    buffer.effectCoverage = 1.0f;
    mNightVisionBuffer.Update(buffer);
    mNightVisionBuffer.BindPS(0);

    mSampler.BindPS(0);
    mNoiseTexture.BindPS(1);

    mRenderTarget.BindPS(0);
    mScreenMeshBuffer.Render();
    mRenderTarget.UnbindPS(0);
}
