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
	void RenderDepthMap();
	void RenderScene();

	struct EarthTransformData
	{
		WallG::Math::Matrix4 world;
		WallG::Math::Matrix4 wvp[2];
		WallG::Math::Vector3 viewPosition;
		float pading;
		//int useShadow;
	};

	struct Settings
	{
		float specularMapWeight = 1.0f;
		float bumpMapWeight = 0.2f;
		float normalMapWeight = 1.0f;
		float padding;
	};

	WallG::Graphics::Camera mDefaultCamera;
	WallG::Graphics::Camera mLightCamera;
	WallG::Graphics::Camera* mActiveCamera = nullptr;

	WallG::Graphics::RenderTarget mDepthRengerTarget;
	WallG::Graphics::RenderTarget mBaseRenderTarget;
	WallG::Graphics::RenderTarget mBloomRenderTarget;
	WallG::Graphics::RenderTarget mBlurRenderTarget;

	WallG::Graphics::TypedConstantBuffer<WallG::Math::Matrix4> mDepthMapTransformBuffer;
	WallG::Graphics::TypedConstantBuffer<Settings> mSettingsBuffer;
	WallG::Graphics::TypedConstantBuffer<EarthTransformData> mEarthTransformBuffer;
	WallG::Graphics::TypedConstantBuffer<WallG::Graphics::DirectionalLight> mLightBuffer;
	WallG::Graphics::TypedConstantBuffer<WallG::Graphics::Material> mMaterialBuffer;

	WallG::Graphics::DirectionalLight mDirectionalLight;
	WallG::Graphics::Material mMaterial;

	WallG::Graphics::Mesh mEarthMesh;
	WallG::Graphics::MeshBuffer mEarthMeshBuffer;
	WallG::Graphics::Mesh mPlaneMesh;
	WallG::Graphics::MeshBuffer mPlaneMeshBuffer;

	WallG::Graphics::VertexShader mDepthMapVertexShader;
	WallG::Graphics::PixelShader mDepthMapPixelShader;

	WallG::Graphics::VertexShader mStandardVertexShader;
	WallG::Graphics::PixelShader mStandardPixelShader;

	WallG::Graphics::Sampler mSampler;
	WallG::Graphics::Texture mEarthDiffuseTexture;
	WallG::Graphics::Texture mEarthSpecularTexture;
	WallG::Graphics::Texture mEarthDisplacmentTexture;
	WallG::Graphics::Texture mEarthNormalTexture;
	WallG::Graphics::Texture mBlankTexture;

	WallG::Math::Vector3 mRotation;
	float mFPS = 0.0f;

	WallG::Graphics::MeshPX mScreenMesh;
	WallG::Graphics::MeshBuffer mScreenMeshBuffer;


	float mLightCameraDistance = 100.0f;
	Settings mSettings;
};


