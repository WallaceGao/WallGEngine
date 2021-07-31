#pragma once
#include <WallG/Inc/WallG.h>
#include <ImGui/Inc/AppLog.h>

class GameState: public WallG::AppState
{
public:
	void Initialize() override;
	void Terminate() override;

	void Update(float deltaTime) override;
	void DebugUI() override;
private:
	AppLog mApplog;

	WallG::ML::Dataset mDataset;
	WallG::ML::LinearRegression mLinReg;

	const int epoch = 10000;

};

