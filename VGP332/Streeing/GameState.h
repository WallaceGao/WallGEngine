#pragma once
#include "SCV.h"

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

	AI::AIWorld world;
	std::vector<std::unique_ptr<SCV>> scvs;
};