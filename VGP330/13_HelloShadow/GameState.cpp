
#include "GameState.h"

using namespace WallG::Graphics;
using namespace WallG::Input;
using namespace WallG::Math;

void GameState::Initialize()
{
	GraphicsSystem::Get()->SetClearColor(Colors::Black);

	mDefaultCamera.SetPosition({ 0.0f, 5.0f, -5.0f });
	mDefaultCamera.SetNearPlane(0.001f);

	mLightCamera.SetNearPlane(1.0f);
	mLightCamera.SetFarPlane(20000.0f);
	mLightCamera.SetAspectRatio(1.0f);

	mActiveCamera = &mDefaultCamera;


	constexpr uint32_t depthMapSize = 4096;
	mDepthRengerTarget.Initialize(depthMapSize, depthMapSize, Texture::Format::RGBA_F32);

	mEarthMesh = MeshBuilder::CreatSphere(256, 256, 1.0f);
	mEarthMeshBuffer.Initialize(mEarthMesh);

	mPlaneMesh = MeshBuilder::CreatPlane(10,10,10.0f);
	mPlaneMeshBuffer.Initialize(mPlaneMesh);

	mDepthMapVertexShader.Initialize(L"../../Assets/Shaders/DepthMap.fx", Vertex::Format);
	mDepthMapPixelShader.Initialize(L"../../Assets/Shaders/DepthMap.fx");

	mStandardVertexShader.Initialize(L"../../Assets/Shaders/Standard.fx", BoneVertex::Format);
	mStandardPixelShader.Initialize(L"../../Assets/Shaders/Standard.fx");



	mDepthMapTransformBuffer.Initialize();
	mEarthTransformBuffer.Initialize();
	mSettingsBuffer.Initialize();

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
	mEarthNormalTexture.Terminate();
	mEarthDisplacmentTexture.Terminate();
	mEarthSpecularTexture.Terminate();
	mEarthDiffuseTexture.Terminate();
	mSampler.Terminate();
	mMaterialBuffer.Terminate();
	mLightBuffer.Terminate();
	mDepthMapTransformBuffer.Terminate();
	mSettingsBuffer.Terminate();
	mEarthTransformBuffer.Terminate();
	mStandardPixelShader.Terminate();
	mStandardVertexShader.Terminate();
	mDepthMapPixelShader.Terminate();
	mDepthMapVertexShader.Terminate();
	mPlaneMeshBuffer.Terminate();
	mEarthMeshBuffer.Terminate();
	mDepthRengerTarget.Terminate();
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
		mActiveCamera->Walk(moveSpeed * deltaTime);
	if (inputSystem->IsKeyDown(KeyCode::S))
		mActiveCamera->Walk(-moveSpeed * deltaTime);
	if (inputSystem->IsKeyDown(KeyCode::D))
		mActiveCamera->Strafe(moveSpeed * deltaTime);
	if (inputSystem->IsKeyDown(KeyCode::A))
		mActiveCamera->Strafe(-moveSpeed * deltaTime);
	if (inputSystem->IsMouseDown(MouseButton::RBUTTON))
	{
		mActiveCamera->Yaw(inputSystem->GetMouseMoveX() * turnSpeed * deltaTime);
		mActiveCamera->Pitch(inputSystem->GetMouseMoveY() * turnSpeed * deltaTime);
	}

	// Synchronize the light camera to the directional light (cannot look straight up or down!)
	mLightCamera.SetDirection(mDirectionalLight.direction);
	//
	Vector3 newCamaraPosition = -mLightCamera.GetDirection() * mLightCameraDistance;
	mLightCamera.SetPosition(newCamaraPosition);
}

void GameState::Render()
{
	RenderDepthMap();
	// Capture a screen shot - 1st Pass
	RenderScene();
}

void GameState::DebugUI()
{
	ImGui::Begin("Debug Info", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
	ImGui::Text("fps: %f", mFPS);

	//Default Open 
	if (ImGui::CollapsingHeader("Camera", ImGuiTreeNodeFlags_DefaultOpen))
	{
		bool useLightCamera = (mActiveCamera == &mLightCamera);
		if (ImGui::Checkbox("Use Light Camera", &useLightCamera))
		{
			mActiveCamera = useLightCamera ? &mLightCamera : &mDefaultCamera;
		}
		ImGui::DragFloat("Light Camera Distance", &mLightCameraDistance, 1.0f, 0.0f, 1000.0f);
	}

	if (ImGui::CollapsingHeader("Light", ImGuiTreeNodeFlags_DefaultOpen))
	{
		if (ImGui::DragFloat3("Light Direction", &mDirectionalLight.direction.x, 0.01f, -1.0f, 1.0f))
			mDirectionalLight.direction = Normalize(mDirectionalLight.direction);
		ImGui::ColorEdit3("Light Ambient", &mDirectionalLight.ambient.x);
		ImGui::ColorEdit3("Light Diffuse", &mDirectionalLight.diffuse.x);
		ImGui::ColorEdit3("Light Specular", &mDirectionalLight.specular.x);
	}

	if (ImGui::CollapsingHeader("Material", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::ColorEdit3("Material Ambient", &mMaterial.ambient.x);
		ImGui::ColorEdit3("Material Diffuse", &mMaterial.diffuse.x);
		ImGui::ColorEdit3("Material Specular", &mMaterial.specular.x);
		ImGui::DragFloat("Material Power", &mMaterial.Power, 0.1f, 1.0f, 100.0f);
	}

	if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::DragFloat3("Rotation", (float*)&mRotation, 0.01f);
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

	ImGui::Image(mDepthRengerTarget.GetRawData(), { 200.0f,200.0f });

	ImGui::End();
}

void GameState::RenderDepthMap()
{
	mDepthRengerTarget.BeginRender(Colors::Black);

	Matrix4 matView = mLightCamera.GetViewMatrix();
	Matrix4 matProj = mLightCamera.GetProjectionMatrix();

	mDepthMapTransformBuffer.BindVS(0);

	Matrix4 matWorld = Matrix4::Translation({ 0.0f, 2.5f, 0.0f }) * Matrix4::RotationX(mRotation.x) * Matrix4::RotationY(mRotation.y);
	Matrix4 wvp = Transpose(matWorld * matView * matProj);
	mDepthMapTransformBuffer.Update(wvp);

	mDepthMapVertexShader.Bind();
	mDepthMapPixelShader.Bind();

	mEarthDisplacmentTexture.BindVS(0);
	mSampler.BindVS(0);

	mEarthMeshBuffer.Render();

	mDepthRengerTarget.EndRender();
}

void GameState::RenderScene()
{

	Matrix4 matView = mActiveCamera->GetViewMatrix();
	Matrix4 matProj = mActiveCamera->GetProjectionMatrix();

	Matrix4 matViewLight = mLightCamera.GetViewMatrix();
	Matrix4 matProjLight = mLightCamera.GetProjectionMatrix();

	EarthTransformData standardTransformData;
	standardTransformData.viewPosition = mActiveCamera->GetPosition();
	mEarthTransformBuffer.BindVS(0);
	mEarthTransformBuffer.BindPS(0);

	mLightBuffer.Update(mDirectionalLight);
	mLightBuffer.BindVS(1);
	mLightBuffer.BindPS(1);

	mMaterialBuffer.Update(mMaterial);
	mMaterialBuffer.BindVS(2);
	mMaterialBuffer.BindPS(2);

	mSettingsBuffer.Update(mSettings);
	mSettingsBuffer.BindVS(3);
	mSettingsBuffer.BindPS(3);

	Matrix4 matWorld = Matrix4::Translation({ 0.0f, 2.5f, 0.0f }) * Matrix4::RotationX(mRotation.x) * Matrix4::RotationY(mRotation.y);
	standardTransformData.world = Transpose(matWorld);
	standardTransformData.wvp[0] = Transpose(matWorld * matView * matProj);
	standardTransformData.wvp[1] = Transpose(matWorld * matViewLight * matProjLight);
	mEarthTransformBuffer.Update(standardTransformData);

	mStandardVertexShader.Bind();
	mStandardPixelShader.Bind();

	mSampler.BindVS(0);
	mSampler.BindPS(0);

	mEarthDiffuseTexture.BindPS(0);
	mEarthSpecularTexture.BindPS(1);
	mEarthDisplacmentTexture.BindVS(2);
	mEarthNormalTexture.BindPS(3);
	mDepthRengerTarget.BindPS(4);

	mEarthMeshBuffer.Render();

	standardTransformData.world = Transpose(Matrix4::Indentity);
	standardTransformData.wvp[0] = Transpose(matView * matProj);
	standardTransformData.wvp[1] = Transpose(matViewLight * matProjLight);
	mEarthTransformBuffer.Update(standardTransformData);
	

	mPlaneMeshBuffer.Render();

	//Texture::UnbindPS(4);
	//
	//SimpleDraw::AddLine({ 0,0,0 }, { 5,0,0 }, Colors::Red);
	//SimpleDraw::AddLine({ 0,0,0 }, { 0,5,0 }, Colors::Green);
	//SimpleDraw::AddLine({ 0,0,0 }, { 0,0,5 }, Colors::Blue);
	//SimpleDraw::Render(*mActiveCamera);

}





