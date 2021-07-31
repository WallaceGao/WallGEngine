#pragma once
#include <WallG/Inc/WallG.h>

class GameState: public WallG::AppState
{
public:
	void Initialize() override;
	void Terminate() override;
	void Update(float deltaTime) override;
	void Render() override;
private:
	struct TransformData
	{
		WallG::Math::Matrix4 wvp;
	};

	WallG::Graphics::Camera mCamera;

	WallG::Graphics::TypedConstantBuffer<TransformData> mConstantBuffer;

	WallG::Graphics::MeshPC mMesh;
	WallG::Graphics::MashBuffer mMeshBuffer;

	WallG::Graphics::VertexShader mVertexShader;
	WallG::Graphics::PixelShader mPixelShader;

	WallG::Math::Vector3 mPosition;
	WallG::Math::Vector3 mRotation;
};

