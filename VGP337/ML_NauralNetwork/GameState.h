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
	

	WallG::ML::NeuralNetwork mNerualNextwork{ {2,2,1} };
	
	std::vector<float> mInputValues;
	std::vector<float> mOutputValues;

	AppLog mAppLog;
	int mTrainTimes = 1000;
	float mValue1 = 1.0f;
	float mValue2 = 0.0f;
	float mOutPutValue = 1.0f;
};

