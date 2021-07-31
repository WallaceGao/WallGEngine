#include "GameState.h"

using namespace WallG::Graphics;
using namespace WallG::Input;
using namespace WallG::Math;

void GameState::Initialize()
{
	GraphicsSystem::Get()->SetClearColor(Colors::Black);

	mCamera.SetPosition({ 0.0f, 0.0f, -12.0f });

	mBaseRenderTarget.Initialize(
		GraphicsSystem::Get()->GetBackBufferWidth(),
		GraphicsSystem::Get()->GetBackBufferHeight()
	);
	mBloomRenderTarget.Initialize(
		GraphicsSystem::Get()->GetBackBufferWidth(),
		GraphicsSystem::Get()->GetBackBufferHeight()
	);
	mBlurRenderTarget.Initialize(
		GraphicsSystem::Get()->GetBackBufferWidth(),
		GraphicsSystem::Get()->GetBackBufferHeight()
	);

	mEarthMesh = MeshBuilder::CreatSphere(256, 256, 1.0f);
	mEarthMeshBuffer.Initialize(mEarthMesh);

	mSunMesh = MeshBuilder::CreatSpherePX(256, 256, 5.0f);
	mSunMeshBuffer.Initialize(mSunMesh);

	mTexturingVertexShader.Initialize(L"../../Assets/Shaders/DoTexturing.fx", VertexPX::Format);
	mTexturingPixelShader.Initialize(L"../../Assets/Shaders/DoTexturing.fx");

	mEarthVertexShader.Initialize(L"../../Assets/Shaders/Earth.fx", Vertex::Format);
	mEarthPixelShader.Initialize(L"../../Assets/Shaders/Earth.fx");

	mGaussianBlurVertexShader.Initialize(L"../../Assets/Shaders/GaussianBlur.fx", VertexPX::Format);
	mGaussianBlurXPixelShader.Initialize(L"../../Assets/Shaders/GaussianBlur.fx", "BlurXPS");
	mGaussianBlurYPixelShader.Initialize(L"../../Assets/Shaders/GaussianBlur.fx", "BlurYPS");

	mBloomVertexShader.Initialize(L"../../Assets/Shaders/Bloom.fx", VertexPX::Format);
	mBloomPixelShader.Initialize(L"../../Assets/Shaders/Bloom.fx");

	mEarthTransformBuffer.Initialize();
	mTexturingTransformBuffer.Initialize();
	mBlurSettingsBuffer.Initialize();
	mLightBuffer.Initialize();
	mMaterialBuffer.Initialize();

	mDirectionalLight.direction = Normalize({ 1.0f, -1.0f, 1.0f });
	mDirectionalLight.ambient = { 0.2f, 0.2f, 0.2f, 1.0f };
	mDirectionalLight.diffuse = { 0.9f, 0.9f, 0.9f, 1.0f };
	mDirectionalLight.specular = { 0.5f, 0.5f, 0.5f, 1.0f };

	mMaterial.ambient = { 0.2f, 0.2f, 0.2f, 1.0f };
	mMaterial.diffuse = { 0.85f, 0.85f, 0.85f, 1.0f };
	mMaterial.specular = { 0.5f, 0.5f, 0.5f, 1.0f };
	mMaterial.Power = 10.0f;

	mSampler.Initialize(Sampler::Filter::Linear, Sampler::AddressMode::Wrap);
	mEarthDiffuseTexture.Initialize(L"../../Assets/Images/EarthTextures/earth.jpg");
	mEarthSpecularTexture.Initialize(L"../../Assets/Images/EarthTextures/earth_spec.jpg");
	mEarthDisplacmentTexture.Initialize(L"../../Assets/Images/EarthTextures/earth_bump.jpg");
	mEarthNormalTexture.Initialize(L"../../Assets/Images/EarthTextures/earth_normal.jpg");
	mSunDiffuseTexture.Initialize(L"../../Assets/Images/SolarTextures/2k_sun.jpg");

	//
	// (-1,+1)---------------(+1,+1)
	//    |          ^          |
	//    |          +->        |
	//    |                     |
	// (-1,-1)---------------(+1,-1)
	//
	mScreenMesh.vertices.push_back({ {-1.0f, -1.0f, 0.0f }, { 0.0f, 1.0f } });
	mScreenMesh.vertices.push_back({ {-1.0f, +1.0f, 0.0f }, { 0.0f, 0.0f } });
	mScreenMesh.vertices.push_back({ {+1.0f, +1.0f, 0.0f }, { 1.0f, 0.0f } });
	mScreenMesh.vertices.push_back({ {+1.0f, -1.0f, 0.0f }, { 1.0f, 1.0f } });

	mScreenMesh.indices.push_back(0);
	mScreenMesh.indices.push_back(1);
	mScreenMesh.indices.push_back(2);

	mScreenMesh.indices.push_back(0);
	mScreenMesh.indices.push_back(2);
	mScreenMesh.indices.push_back(3);

	mScreenMeshBuffer.Initialize(mScreenMesh);
}

void GameState::Terminate()
{
	mScreenMeshBuffer.Terminate();
	mSunDiffuseTexture.Terminate();
	mEarthNormalTexture.Terminate();
	mEarthDisplacmentTexture.Terminate();
	mEarthSpecularTexture.Terminate();
	mEarthDiffuseTexture.Terminate();
	mSampler.Terminate();
	mMaterialBuffer.Terminate();
	mLightBuffer.Terminate();
	mBlurSettingsBuffer.Terminate();
	mTexturingTransformBuffer.Terminate();
	mEarthTransformBuffer.Terminate();
	mBloomPixelShader.Terminate();
	mBloomVertexShader.Terminate();
	mGaussianBlurYPixelShader.Terminate();
	mGaussianBlurXPixelShader.Terminate();
	mGaussianBlurVertexShader.Terminate();
	mEarthPixelShader.Terminate();
	mEarthVertexShader.Terminate();
	mTexturingPixelShader.Terminate();
	mTexturingVertexShader.Terminate();
	mSunMeshBuffer.Terminate();
	mEarthMeshBuffer.Terminate();
	mBlurRenderTarget.Terminate();
	mBloomRenderTarget.Terminate();
	mBaseRenderTarget.Terminate();
}

void GameState::Update(float deltaTime)
{
	mFPS = 1.0f / deltaTime;

	auto inputSystem = InputSystem::Get();

	if (inputSystem->IsKeyDown(KeyCode::LEFT))
		mRotation.y += deltaTime;
	if (inputSystem->IsKeyDown(KeyCode::RIGHT))
		mRotation.y -= deltaTime;

	if (inputSystem->IsKeyDown(KeyCode::UP))
		mRotation.x += deltaTime;
	if (inputSystem->IsKeyDown(KeyCode::DOWN))
		mRotation.x -= deltaTime;

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
	// Capture a screen shot - 1st Pass
	RenderScene();

	// Capture just the bright pixels for bloom - 2nd Pass
	RenderBrightness();

	// Apply Gaussian blur to the bloom pixels
	ApplyBlur();

	// Composite both textures - 3rd Pass
	PostProcess();
}

void GameState::DebugUI()
{
	ImGui::Begin("Debug Info", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
	ImGui::Text("fps: %f", mFPS);
	if (ImGui::DragFloat3("Light Direction", &mDirectionalLight.direction.x, 0.01f, -1.0f, 1.0f))
		mDirectionalLight.direction = Normalize(mDirectionalLight.direction);
	ImGui::ColorEdit3("Light Ambient", &mDirectionalLight.ambient.x);
	ImGui::ColorEdit3("Light Diffuse", &mDirectionalLight.diffuse.x);
	ImGui::ColorEdit3("Light Specular", &mDirectionalLight.specular.x);
	ImGui::ColorEdit3("Material Ambient", &mMaterial.ambient.x);
	ImGui::ColorEdit3("Material Diffuse", &mMaterial.diffuse.x);
	ImGui::ColorEdit3("Material Specular", &mMaterial.specular.x);
	ImGui::DragFloat("Material Power", &mMaterial.Power, 0.1f, 1.0f, 100.0f);
	ImGui::DragFloat3("Rotation", (float*)&mRotation, 0.01f);

	ImGui::DragInt("Blur Iterations", &mBlurIterations, 1, 0, 100);
	ImGui::DragFloat("Blur Saturation", &mBlurSaturation, 0.001f, 0.0f, 1.0f);
	ImGui::End();
}

void GameState::RenderScene()
{
	mBaseRenderTarget.BeginRender();

	Matrix4 matView = mCamera.GetViewMatrix();
	Matrix4 matProj = mCamera.GetProjectionMatrix();

	EarthTransformData earthTransformData;
	earthTransformData.viewPosition = mCamera.GetPosition();
	mEarthTransformBuffer.BindVS(0);
	mEarthTransformBuffer.BindPS(0);

	mLightBuffer.Update(mDirectionalLight);
	mLightBuffer.BindVS(1);
	mLightBuffer.BindPS(1);

	mMaterialBuffer.Update(mMaterial);
	mMaterialBuffer.BindVS(2);
	mMaterialBuffer.BindPS(2);

	Matrix4 matWorld = Matrix4::Translation({ 0.0f, 0.0f, -8.0f }) * Matrix4::RotationX(mRotation.x) * Matrix4::RotationY(mRotation.y);
	earthTransformData.world = Transpose(matWorld);
	earthTransformData.wvp = Transpose(matWorld * matView * matProj);
	mEarthTransformBuffer.Update(earthTransformData);

	mEarthVertexShader.Bind();
	mEarthPixelShader.Bind();

	mSampler.BindVS(0);
	mSampler.BindPS(0);

	mEarthDiffuseTexture.BindPS(0);
	mEarthSpecularTexture.BindPS(1);
	mEarthDisplacmentTexture.BindVS(2);
	mEarthNormalTexture.BindPS(3);

	mEarthMeshBuffer.Render();

	mTexturingTransformBuffer.Update(Transpose(matView * matProj));
	mTexturingTransformBuffer.BindVS(0);

	mTexturingVertexShader.Bind();
	mTexturingPixelShader.Bind();

	mSunDiffuseTexture.BindPS(0);
	mSunMeshBuffer.Render();

	mBaseRenderTarget.EndRender();
}

void GameState::RenderBrightness()
{
	mBloomRenderTarget.BeginRender();

	Matrix4 matView = mCamera.GetViewMatrix();
	Matrix4 matProj = mCamera.GetProjectionMatrix();

	Matrix4 matWorld = Matrix4::Translation({ 0.0f, 0.0f, -8.0f }) * Matrix4::RotationX(mRotation.x) * Matrix4::RotationY(mRotation.y);
	mTexturingTransformBuffer.Update(Transpose(matWorld * matView * matProj));
	mTexturingTransformBuffer.BindVS(0);

	mTexturingVertexShader.Bind();
	mTexturingPixelShader.Bind();

	mSampler.BindPS(0);

	mBlankTexture.BindPS(0);
	mEarthMeshBuffer.Render();

	mTexturingTransformBuffer.Update(Transpose(matView * matProj));

	mSunDiffuseTexture.BindPS(0);
	mSunMeshBuffer.Render();

	mBloomRenderTarget.EndRender();
}

void GameState::ApplyBlur()
{
	auto graphicsSystem = GraphicsSystem::Get();
	Vector4 settings;
	settings.x = static_cast<float>(graphicsSystem->GetBackBufferWidth());
	settings.y = static_cast<float>(graphicsSystem->GetBackBufferHeight());
	settings.z = mBlurSaturation;
	mBlurSettingsBuffer.Update(settings);
	mBlurSettingsBuffer.BindPS(0);

	mGaussianBlurVertexShader.Bind();
	mSampler.BindPS(0);

	for (int i = 0; i < mBlurIterations; ++i)
	{
		// Horizontal blur
		mBlurRenderTarget.BeginRender();
		mBloomRenderTarget.BindPS(0);
		mGaussianBlurXPixelShader.Bind();
		mScreenMeshBuffer.Render();
		Texture::UnbindPS(0);
		mBlurRenderTarget.EndRender();

		// Vertical blur
		mBloomRenderTarget.BeginRender();
		mBlurRenderTarget.BindPS(0);
		mGaussianBlurYPixelShader.Bind();
		mScreenMeshBuffer.Render();
		Texture::UnbindPS(0);
		mBloomRenderTarget.EndRender();
	}
}

void GameState::PostProcess()
{
	mBloomVertexShader.Bind();
	mBloomPixelShader.Bind();

	mSampler.BindPS(0);

	mBaseRenderTarget.BindPS(0);
	mBloomRenderTarget.BindPS(1);

	mScreenMeshBuffer.Render();

	Texture::UnbindPS(0);
	Texture::UnbindPS(1);
}




