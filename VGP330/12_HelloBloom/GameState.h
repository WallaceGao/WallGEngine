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
	void RenderBrightness();
	void ApplyBlur();
	void PostProcess();

	struct EarthTransformData
	{
		WallG::Math::Matrix4 world;
		WallG::Math::Matrix4 wvp;
		WallG::Math::Vector3 viewPosition;
		float padding;
	};

	WallG::Graphics::Camera mCamera;

	WallG::Graphics::RenderTarget mBaseRenderTarget;
	WallG::Graphics::RenderTarget mBloomRenderTarget;
	WallG::Graphics::RenderTarget mBlurRenderTarget;

	WallG::Graphics::TypedConstantBuffer<EarthTransformData> mEarthTransformBuffer;
	WallG::Graphics::TypedConstantBuffer<WallG::Math::Matrix4> mTexturingTransformBuffer;
	WallG::Graphics::TypedConstantBuffer<WallG::Math::Vector4> mBlurSettingsBuffer;
	WallG::Graphics::TypedConstantBuffer<WallG::Graphics::DirectionalLight> mLightBuffer;
	WallG::Graphics::TypedConstantBuffer<WallG::Graphics::Material> mMaterialBuffer;

	WallG::Graphics::DirectionalLight mDirectionalLight;
	WallG::Graphics::Material mMaterial;

	WallG::Graphics::Mesh mEarthMesh;
	WallG::Graphics::MeshBuffer mEarthMeshBuffer;
	WallG::Graphics::MeshPX mSunMesh;
	WallG::Graphics::MeshBuffer mSunMeshBuffer;

	WallG::Graphics::VertexShader mTexturingVertexShader;
	WallG::Graphics::PixelShader mTexturingPixelShader;

	WallG::Graphics::VertexShader mEarthVertexShader;
	WallG::Graphics::PixelShader mEarthPixelShader;

	WallG::Graphics::VertexShader mGaussianBlurVertexShader;
	WallG::Graphics::PixelShader mGaussianBlurXPixelShader;
	WallG::Graphics::PixelShader mGaussianBlurYPixelShader;

	WallG::Graphics::VertexShader mBloomVertexShader;
	WallG::Graphics::PixelShader mBloomPixelShader;

	WallG::Graphics::Sampler mSampler;
	WallG::Graphics::Texture mEarthDiffuseTexture;
	WallG::Graphics::Texture mEarthSpecularTexture;
	WallG::Graphics::Texture mEarthDisplacmentTexture;
	WallG::Graphics::Texture mEarthNormalTexture;
	WallG::Graphics::Texture mSunDiffuseTexture;
	WallG::Graphics::Texture mBlankTexture;

	WallG::Math::Vector3 mRotation;
	float mFPS = 0.0f;

	WallG::Graphics::MeshPX mScreenMesh;
	WallG::Graphics::MeshBuffer mScreenMeshBuffer;

	int mBlurIterations = 10;
	float mBlurSaturation = 0.28f;
};

//#pragma once
//#include <WallG/Inc/WallG.h>
//
//class GameState: public WallG::AppState
//{
//public:
//	void Initialize() override;
//	void Terminate() override;
//	void Update(float deltaTime) override;
//	void Render() override;
//	void DebugUI() override;
//private:
//	void RenderScene();
//	void PostProcessOldStyle();
//	void PostProcessThermal();
//	void PostProcessNightVision();
//
//	struct SkyBox
//	{
//		WallG::Math::Matrix4 mvp;
//	};
//
//	struct ThermalBuffer
//	{
//		float vxOffset;
//		float padding[3];
//	};
//
//	struct NightVisionBuffer
//	{
//		float elapsedTime;
//		float luminanceThreshold;
//		float colorAmplification;
//		float effectCoverage;
//	};
//
//	struct TransformData
//	{
//		WallG::Math::Matrix4 world;
//		WallG::Math::Matrix4 wvp;
//		WallG::Math::Vector3 viewPostion;
//		float padding;
//	};
//
//	struct Settings
//	{
//		float specularMapWeight= 1.0f;
//		float bumpMapWeight = 0.2f;
//		float normalMapWeight = 1.0f;
//		float padding;
//	};
//
//	WallG::Graphics::Camera mCamera;
//
//	WallG::Graphics::RenderTarget mRenderTarget;
//	//Post Process
//
//
//	// SkyBox
//	WallG::Graphics::TypedConstantBuffer<SkyBox> mConstantBufferSkyBox;
//	WallG::Graphics::MeshPX mSkyboxMesh;
//	WallG::Graphics::MeshBuffer mSkyboxMeshBuffer;
//	WallG::Graphics::Texture mSkyboxTexture;
//
//	WallG::Graphics::VertexShader mVertexShaderSkyBox;
//	WallG::Graphics::PixelShader mPixelShaderSkyBox;
//
//	//Sun
//	float mSunScale = 0;
//	WallG::Graphics::MeshBuffer mSunMeshBuffer;
//
//	WallG::Graphics::Texture mSunTexture;
//	WallG::Math::Vector3 mSumRotation;
//
//	// Earth
//	WallG::Graphics::TypedConstantBuffer<TransformData> mConstantBuffer;
//	WallG::Graphics::TypedConstantBuffer<Settings> mSettingsBuffer;
//	WallG::Graphics::TypedConstantBuffer<WallG::Graphics::DirectionalLight> mLightBuffer;
//	WallG::Graphics::TypedConstantBuffer<WallG::Graphics::Material> mMaterialBuffer;
//
//	WallG::Graphics::Texture mEarthTexture;
//	WallG::Graphics::Texture mEarthSpec;
//	WallG::Graphics::Texture mEarthDisplace;
//	WallG::Graphics::Texture mEarthNormal;
//	WallG::Graphics::Texture mEarthClouds;
//	WallG::Graphics::Texture mEarthNight;
//
//	WallG::Graphics::BlendState mBlendStateClouds;
//
//	WallG::Graphics::DirectionalLight mDirctionalLight;
//	WallG::Graphics::Material mMaterial;
//
//	WallG::Graphics::Mesh mSphere;
//	WallG::Graphics::MeshBuffer mMeshBuffer;
//
//	WallG::Graphics::VertexShader mVertexShader;
//	WallG::Graphics::PixelShader mPixelShader;
//	WallG::Graphics::VertexShader mVertexSharderCloud;
//	WallG::Graphics::PixelShader mPixelShaderCloud;
//
//	WallG::Graphics::Sampler mSampler;
//
//	WallG::Math::Vector3 mPosition;
//	WallG::Math::Vector3 mRotation;
//	float mFPS = 0.0f;
//	float mCloudRota = 0.0f;
//	float mEarthRota = 0.0f;
//	float mCloudRotaSpeed = 0.5f;
//	float mEarthRotaSpeed = 0.1f;
//	int postProcess = 0;
//
//	Settings mSettings;
//
//
//	WallG::Graphics::MeshPX mScreenMash;
//	WallG::Graphics::MeshBuffer mScreenMeshBuffer;
//
//};

