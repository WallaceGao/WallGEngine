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
		WallG::Math::Matrix4 world;
		WallG::Math::Matrix4 wvp;
		WallG::Math::Vector3 viewPostion;
		float padding;
	};

	WallG::Graphics::Camera mCamera;

	WallG::Graphics::TypedConstantBuffer<TransformData> mConstantBuffer;
	WallG::Graphics::TypedConstantBuffer<WallG::Graphics::DirectionalLight> mLightBuffer;
	WallG::Graphics::TypedConstantBuffer<WallG::Graphics::Material> mMaterialBuffer;

	WallG::Graphics::DirectionalLight mDirctionalLight;
	WallG::Graphics::Material mMaterial;

	WallG::Graphics::MeshPN mSphere;
	WallG::Graphics::MeshBuffer mMeshBuffer;

	WallG::Graphics::VertexShader mVertexShader[2];
	WallG::Graphics::PixelShader mPixelShader[2];

	WallG::Math::Vector3 mPosition;
	WallG::Math::Vector3 mRotation;
	float mFPS = 0.0f;
};

