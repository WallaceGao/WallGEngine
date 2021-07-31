#include "GameState.h"

using namespace WallG::Graphics;
using namespace WallG::Input;
using namespace WallG::Math;

namespace
{
	Camera MatrixReflection(const Camera& camera, float height )
	{
		Vector3 position, direction;

		// Setup the position of the camera in the world.
		// For planar reflection invert the Y position of the camera.
		position.x = camera.GetPosition().x;
		position.y = -camera.GetPosition().y + (height * 2.0f);
		position.z = camera.GetPosition().z;

		//// Calculate the rotation in radians.
		//radians = m_rotationY * 0.0174532925f;
		direction.x = camera.GetDirection().x;
		direction.y = -camera.GetDirection().y;
		direction.z = camera.GetDirection().z;

		//// Create the view matrix from the three vectors.
		//D3DXMatrixLookAtLH(&m_reflectionViewMatrix, &position, &lookAt, &up);
		Camera reflectionCamera;
		reflectionCamera.SetDirection(direction);
		reflectionCamera.SetPosition(position);
		//reflectionCamera.SetDirection(Normalize(lookAt - position));
		//reflectionCamera.GetViewMatrix();
		return reflectionCamera;
	}
}

void GameState::Initialize()
{
	GraphicsSystem::Get()->SetClearColor(Colors::Black);

	mDefaultCamera.SetPosition({ 0.0f, 25.0f, -100.0f });
	mDefaultCamera.SetNearPlane(0.001f);

	mReflectionCamera = MatrixReflection(mDefaultCamera, mHeight);

	mActiveCamera = &mDefaultCamera;

	

	constexpr uint32_t depthMapSize = 4096;
	mReflectRenderTarget.Initialize(depthMapSize, depthMapSize, Texture::Format::RGBA_F32);

	mPlaneMesh = MeshBuilder::CreatPlanePX(2,2,200.0f);
	mPlaneMeshBuffer.Initialize(mPlaneMesh);
	ModelLoader::LoadObj("../../Assets/Model/trident_a10/trident_a10.obj", 0.05f, mModelMesh);
	mModelMeshBuffer.Initialize(mModelMesh);

	mGroundMapVertexShader.Initialize(L"../../Assets/Shaders/Reflection.fx", VertexPX::Format);
	mGroundMapPixelShader.Initialize(L"../../Assets/Shaders/Reflection.fx");

	mModelVertexShader.Initialize(L"../../Assets/Shaders/DoTexturing.fx", Vertex::Format);
	mModelPixelShader.Initialize(L"../../Assets/Shaders/DoTexturing.fx");

	mDirectionalLight.direction = Normalize({ 1.0f, -1.0f, 1.0f });
	mDirectionalLight.ambient = { 0.2f, 0.2f, 0.2f, 1.0f };
	mDirectionalLight.diffuse = { 0.9f, 0.9f, 0.9f, 1.0f };
	mDirectionalLight.specular = { 0.5f, 0.5f, 0.5f, 1.0f };

	mMaterial.ambient = { 0.2f, 0.2f, 0.2f, 1.0f };
	mMaterial.diffuse = { 0.85f, 0.85f, 0.85f, 1.0f };
	mMaterial.specular = { 0.5f, 0.5f, 0.5f, 1.0f };
	mMaterial.Power = 10.0f;

	mSampler.Initialize(Sampler::Filter::Linear, Sampler::AddressMode::Clamp);
	mGroundTexture.Initialize("../../Assets/Images/Grass.jpg");
	mSpaceShipTexture.Initialize("../../Assets/Model/trident_a10/trident_no_decal_diffuse.tif");
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
	mReflectionBuffer.Initialize();
	mReflectionTransform.Initialize();
	mConstantBuffer.Initialize();

}

void GameState::Terminate()
{
	mConstantBuffer.Terminate();
	mReflectionTransform.Terminate();
	mReflectionBuffer.Terminate();
	mScreenMeshBuffer.Terminate();

	mSpaceShipTexture.Terminate();
	mGroundTexture.Terminate();
	mSampler.Terminate();

	mGroundMapPixelShader.Terminate();
	mGroundMapVertexShader.Terminate();
	mModelPixelShader.Terminate();
	mModelVertexShader.Terminate();

	mModelMeshBuffer.Terminate();
	mPlaneMeshBuffer.Terminate();
	mReflectRenderTarget.Terminate();
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

	const float moveSpeed = 10.0f * (inputSystem->IsKeyDown(KeyCode::LSHIFT)? 10.0f: 1.0f);
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


}

void GameState::Render()
{
	RenderReflection();
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
		bool useReflectionCamera = (mActiveCamera == &mReflectionCamera);
		if (ImGui::Checkbox("Use Reflection Camera", &useReflectionCamera))
		{
			mActiveCamera = useReflectionCamera ? &mReflectionCamera : &mDefaultCamera;
		}
		ImGui::DragFloat("Light Camera Distance", &mLightCameraDistance, 1.0f, 0.0f, 1000.0f);
	}

	if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::DragFloat3("Rotation", (float*)&mRotation, 0.01f);
	}

	static bool normalMap = true;
	static bool specularMap = true;

	ImGui::End();
}

void GameState::RenderScene()
{

	Matrix4 matView = mActiveCamera->GetViewMatrix();
	Matrix4 matProj = mActiveCamera->GetProjectionMatrix();
	TransformData transformData;

	Matrix4 matWorld = Matrix4::Translation({ 0.0f, 10.0f, 0.0f }) * Matrix4::RotationX(mRotation.x) * Matrix4::RotationY(mRotation.y);
	transformData.wvp = Transpose(matWorld * matView * matProj);
	mConstantBuffer.Update(transformData);
	mConstantBuffer.BindVS(0);
	mModelVertexShader.Bind();
	mModelPixelShader.Bind();
	
	mSpaceShipTexture.BindPS(0);
	mSampler.BindPS(0);
	
	mModelMeshBuffer.Render();

	mReflectionCamera = MatrixReflection(mDefaultCamera, mHeight);
	Matrix4 matReflection = mReflectionCamera.GetViewMatrix();

	ReflectionTransformData reflectionTransformData;
	ReflectionBufferData reflectionBufferData;

	matWorld = Matrix4::Translation({ 0.0f, 0.0f, 0.0f }) * Matrix4::RotationX(mRotation.x) * Matrix4::RotationY(mRotation.y);

	reflectionTransformData.world = Transpose(matWorld);
	reflectionTransformData.view = Transpose(matView);
	reflectionTransformData.projection = Transpose(matProj);
	mReflectionTransform.Update(reflectionTransformData);
	mReflectionTransform.BindVS(0);

	reflectionBufferData.reflectionMatrix = Transpose(matReflection);
	mReflectionBuffer.Update(reflectionBufferData);
	mReflectionBuffer.BindVS(1);

	mSampler.BindPS(0);

	mGroundMapVertexShader.Bind();
	mGroundMapPixelShader.Bind();
	mGroundTexture.BindPS(0);
	mReflectRenderTarget.BindPS(1);

	mPlaneMeshBuffer.Render();

	SimpleDraw::AddLine({ 0,0,0 }, { 5,0,0 }, Colors::Red);
	SimpleDraw::AddLine({ 0,0,0 }, { 0,5,0 }, Colors::Green);
	SimpleDraw::AddLine({ 0,0,0 }, { 0,0,5 }, Colors::Blue);
	SimpleDraw::Render(*mActiveCamera);
}

void GameState::RenderReflection()
{
	mReflectRenderTarget.BeginRender();
	
	Matrix4 matView = mActiveCamera->GetViewMatrix();
	Matrix4 matProj = mActiveCamera->GetProjectionMatrix();
	TransformData transformData;

	Matrix4 matWorld = Matrix4::Translation({ 0.0f, 10.0f, 0.0f }) * Matrix4::RotationX(mRotation.x) * Matrix4::RotationY(mRotation.y);
	transformData.wvp = Transpose(matWorld * matView * matProj);
	mConstantBuffer.Update(transformData);
	mConstantBuffer.BindVS(0);
	mModelVertexShader.Bind();
	mModelPixelShader.Bind();

	mSpaceShipTexture.BindPS(0);
	mSampler.BindPS(0);

	mModelMeshBuffer.Render();

	//for (size_t i = 0; i < mPlaneMesh.indices.size(); i += 3)
	//{
	//	auto& a = mPlaneMesh.vertices[mPlaneMesh.indices[i + 0]];
	//	auto& b = mPlaneMesh.vertices[mPlaneMesh.indices[i + 1]];
	//	auto& c = mPlaneMesh.vertices[mPlaneMesh.indices[i + 2]];
	//	SimpleDraw::AddLine(a.position, b.position, Colors::Red);
	//	SimpleDraw::AddLine(b.position, c.position, Colors::Red);
	//	SimpleDraw::AddLine(c.position, a.position, Colors::Red);
	//}
	//SimpleDraw::Render(*mActiveCamera);

	mReflectRenderTarget.EndRender();
}





