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

	struct SkyBox
	{
		WallG::Math::Matrix4 mvp;
	};

	struct TransformData
	{
		WallG::Math::Matrix4 world;
		WallG::Math::Matrix4 wvp;
		WallG::Math::Vector3 viewPostion;
		float padding;
	};

	struct Settings
	{
		float specularMapWeight = 1.0f;
		float bumpMapWeight = 0.2f;
		float normalMapWeight = 1.0f;
		float padding;
	};

	WallG::Graphics::Camera mCamera;

	// SkyBox
	WallG::Graphics::TypedConstantBuffer<SkyBox> mConstantBufferSkyBox;
	WallG::Graphics::MeshPX mSkyboxMesh;
	WallG::Graphics::MeshBuffer mSkyboxMeshBuffer;
	WallG::Graphics::Texture mSkyboxTexture;

	WallG::Graphics::VertexShader mVertexShaderSkyBox;
	WallG::Graphics::PixelShader mPixelShaderSkyBox;

	// Earth
	WallG::Graphics::TypedConstantBuffer<TransformData> mConstantBuffer;
	WallG::Graphics::TypedConstantBuffer<Settings> mSettingsBuffer;
	WallG::Graphics::TypedConstantBuffer<WallG::Graphics::DirectionalLight> mLightBuffer;
	WallG::Graphics::TypedConstantBuffer<WallG::Graphics::Material> mMaterialBuffer;

	WallG::Graphics::Texture mEarthTexture;
	WallG::Graphics::Texture mEarthSpec;
	WallG::Graphics::Texture mEarthDisplace;
	WallG::Graphics::Texture mEarthNormal;
	WallG::Graphics::Texture mEarthClouds;
	WallG::Graphics::Texture mEarthNight;

	WallG::Graphics::BlendState mBlendStateClouds;

	WallG::Graphics::DirectionalLight mDirctionalLight;
	WallG::Graphics::Material mMaterial;

	WallG::Graphics::Mesh mSphere;
	WallG::Graphics::MeshBuffer mMeshBuffer;

	WallG::Graphics::VertexShader mVertexShader;
	WallG::Graphics::PixelShader mPixelShader;
	WallG::Graphics::VertexShader mVertexSharderCloud;
	WallG::Graphics::PixelShader mPixelShaderCloud;

	WallG::Graphics::Sampler mSampler;

	WallG::Math::Vector3 mPosition;
	WallG::Math::Vector3 mRotation;
	float mFPS = 0.0f;
	float mCloudRota = 0.0f;
	float mEarthRota = 0.0f;
	float mCloudRotaSpeed = 0.5f;
	float mEarthRotaSpeed = 0.1f;

	Settings mSettings;
};


