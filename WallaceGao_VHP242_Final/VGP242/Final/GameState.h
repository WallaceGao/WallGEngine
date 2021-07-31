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
	WallG::Graphics::MashBuffer mSkyboxMeshBuffer;
	WallG::Graphics::Texture mSkyboxTexture;

	// Sun
	WallG::Graphics::MeshPX mSunMesh;
	WallG::Graphics::MashBuffer mSunMeshBuffer;
	WallG::Graphics::Texture mSunTexture;

	// Mercury
	WallG::Graphics::MeshPX mMercuryMesh;
	WallG::Graphics::MashBuffer mMercuryMeshBuffer;
	WallG::Graphics::Texture mMercuryTexture;
	float mMercurySpeed = 0.0f;

	// Venus 
	WallG::Graphics::MeshPX mVenusMesh;
	WallG::Graphics::MashBuffer mVenusMeshBuffer;
	WallG::Graphics::Texture mVenusTexture;
	float mVenusSpeed = 0.0f;

	// Earth
	WallG::Graphics::MeshPX mEarthMesh;
	WallG::Graphics::MashBuffer mEarthMeshBuffer;
	WallG::Graphics::Texture mEarthTexture;
	float mEarthSpeed = 0.0f;

	// Mars
	WallG::Graphics::MeshPX mMarsMesh;
	WallG::Graphics::MashBuffer mMarsMeshBuffer;
	WallG::Graphics::Texture mMarsTexture;
	float mMarsSpeed = 0.0f;

	// Jupiter
	WallG::Graphics::MeshPX mJupiterMesh;
	WallG::Graphics::MashBuffer mJupiterMeshBuffer;
	WallG::Graphics::Texture mJupiterTexture;
	float mJupiterSpeed = 0.0f;

	WallG::Graphics::VertexShader mVertexShader;
	WallG::Graphics::PixelShader mPixelShader;

	WallG::Graphics::Sampler mSampler;

	WallG::Math::Vector3 mPosition;
	WallG::Math::Vector3 mRotation;
	float mFPS = 0.0f;

};

