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
	void GenerateData();
	void Train();
	AppLog mApplog;

	WallG::ML::Dataset mDataset;
	WallG::ML::LogisticsRegression mLogReg;

	float learningRate = 0.00045f;
	const int epoch = 10000;
	float b0 = 100.0f;
	float b1 = -0.5f;
	float b2 = 5.0f;
	float minX = 0.0f;
	float maxX = 100.0f;
	float noise = 10.0f;

};

