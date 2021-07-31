#pragma once

#include <WallG/Inc/WallG.h>

class GameState : public WallG::AppState
{
public:
	void Initialize() override;
	void Terminate() override;

	void Update(float deltaTime) override;
	void Render() override;
	void DebugUI() override;

private:
	void RenderScene();
	void RenderReflection();

	struct TransformData
	{
		WallG::Math::Matrix4 wvp;
	};

	struct ReflectionTransformData
	{
		WallG::Math::Matrix4 world;
		WallG::Math::Matrix4 view;
		WallG::Math::Matrix4 projection;
	};

	struct ReflectionBufferData
	{
		WallG::Math::Matrix4 reflectionMatrix;
	};

	WallG::Graphics::Camera mDefaultCamera;
	WallG::Graphics::Camera mReflectionCamera;
	WallG::Graphics::Camera* mActiveCamera = nullptr;

	WallG::Graphics::RenderTarget mBaseRenderTarget;
	WallG::Graphics::RenderTarget mReflectRenderTarget;

	WallG::Graphics::TypedConstantBuffer<TransformData> mConstantBuffer;
	WallG::Graphics::TypedConstantBuffer<ReflectionTransformData> mReflectionTransform;
	WallG::Graphics::TypedConstantBuffer<ReflectionBufferData> mReflectionBuffer;

	WallG::Graphics::DirectionalLight mDirectionalLight;
	WallG::Graphics::Material mMaterial;

	WallG::Graphics::MeshPX mPlaneMesh;
	WallG::Graphics::MeshBuffer mPlaneMeshBuffer;

	WallG::Graphics::VertexShader mGroundMapVertexShader;
	WallG::Graphics::PixelShader mGroundMapPixelShader;

	WallG::Graphics::VertexShader mModelVertexShader;
	WallG::Graphics::PixelShader mModelPixelShader;

	WallG::Graphics::Sampler mSampler;
	WallG::Graphics::Texture mGroundTexture;
	WallG::Graphics::Texture mSpaceShipTexture;

	WallG::Math::Vector3 mRotation;
	float mFPS = 0.0f;

	WallG::Graphics::Mesh mModelMesh;
	WallG::Graphics::MeshBuffer mModelMeshBuffer;

	WallG::Graphics::MeshPX mScreenMesh;
	WallG::Graphics::MeshBuffer mScreenMeshBuffer;

	float mLightCameraDistance = 100.0f;
	float mHeight = 0.0f;
};


