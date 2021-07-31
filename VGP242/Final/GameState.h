#pragma once
#include <WallG/Inc/WallG.h>

class GameState: public WallG::AppState
{
public:
	void Initialize() override;
	void Terminate() override;
	void Update(float deltaTime) override;
	void Render() override;
	void DebugUI() override;
private:
	struct TransformData
	{
		WallG::Math::Matrix4 wvp;
	};

	WallG::Graphics::Camera mCamera;

	WallG::Graphics::TypedConstantBuffer<TransformData> mConstantBuffer;



	// SkyBox
	WallG::Graphics::MeshPX mSkyboxMesh;
	WallG::Graphics::MeshBuffer mSkyboxMeshBuffer;
	WallG::Graphics::Texture mSkyboxTexture;

	WallG::Graphics::MeshPX mSphereMesh;
	WallG::Graphics::MeshBuffer mSphereBuffer;

	// Sun
	WallG::Math::Vector3 mSumRotation;
	WallG::Graphics::Texture mSunTexture;

	// Mercury
	WallG::Graphics::Texture mMercuryTexture;
	WallG::Math::Vector3 mMercuryRotation;
	WallG::Math::Vector3 mSelfMercuryRotation;
	float mMercurySpeed = 0.0f;
	float mSelfMercurySpeed = 0.0f;

	// Venus 
	WallG::Graphics::Texture mVenusTexture;
	WallG::Math::Vector3 mVenusRotation;
	WallG::Math::Vector3 mSelfVenusRotation;
	float mVenusSpeed = 0.0f;
	float mSelfVenusSpeed = 0.0f;

	// Earth
	WallG::Graphics::Texture mEarthTexture;
	WallG::Math::Vector3 mEarthRotation;
	WallG::Math::Vector3 mSelfEarthRotation;
	float mEarthSpeed = 0.0f;
	float mSelfEarthSpeed = 0.0f;

	// Mars
	WallG::Graphics::Texture mMarsTexture;
	WallG::Math::Vector3 mMarsRotation;
	WallG::Math::Vector3 mSelfMarsRotation;
	float mMarsSpeed = 0.0f;
	float mSelfMarsSpeed = 0.0f;

	// Jupiter
	WallG::Graphics::Texture mJupiterTexture;
	WallG::Math::Vector3 mJupiterRotation;
	WallG::Math::Vector3 mSelfJupiterRotation;
	float mJupiterSpeed = 0.0f;
	float mSelfJupiterSpeed = 0.0f;

	//Moon
	WallG::Graphics::Texture mMoonTexture;
	WallG::Math::Vector3 mMoonRotation;
	WallG::Math::Vector3 mSelfMoonRotation;
	float mMoonSpeed = 0.0f;
	float mSelfMoonSpeed = 0.0f;

	WallG::Graphics::VertexShader mVertexShader;
	WallG::Graphics::PixelShader mPixelShader;

	WallG::Graphics::Sampler mSampler;

	WallG::Math::Vector3 mPosition;
	float mFPS = 0.0f;
	
	std::vector<float> mPlanetScale;

};

