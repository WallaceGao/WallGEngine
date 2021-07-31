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
	float mFPS = 0.0f;

	WallG::GameWorld mGameWorld;

	int mNextId = 0;
	float mNextPositionX = 0.0f;
};

// Create ship component
// Create planet component
// Create planet service