#pragma once
#include "WallG/Inc/WallG.h"

class GameState : public WallG::AppState
{
public:
	void Initialize() override;
	void Terminate() override;

	void Update(float deltaTime) override;
	void Render() override;
	void DebugUI() override;



private:
	WallG::GameWorld mGameWorld;

	
	std::vector<WallG::Graphics::TextureId> mTiles;
	int mTextureId = 1;
};