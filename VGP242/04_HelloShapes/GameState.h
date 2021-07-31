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
	int input = 0;
	std::vector< WallG::Graphics::VertexPC > mHeartVertices;
	std::vector< WallG::Graphics::VertexPC > mTriforceVertices;
	std::vector< WallG::Graphics::VertexPC > mWuKongStaffVertices;

	WallG::Graphics::MeshBuffer mVertexBuffer[3];
	//ID3D11InputLayout* mInputLayout = nullptr;
	WallG::Graphics::VertexShader mVertexShader;
	WallG::Graphics::PixelShader mPixelShader;
};