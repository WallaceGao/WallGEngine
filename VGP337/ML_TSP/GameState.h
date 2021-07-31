#pragma once
#include <WallG/Inc/WallG.h>
#include <ImGui/Inc/AppLog.h>

class GameState: public WallG::AppState
{
public:
	void Initialize() override;

	void Update(float deltaTime) override;
	void DebugUI() override;

	
private:
	
	WallG::ML::GenetiAlgorithm mGA;


	AppLog mAppLog;
	bool mInitialized = false;
	bool mDone = false;
};


