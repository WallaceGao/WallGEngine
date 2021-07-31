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
	struct Unit
	{
		ImVec2 position;
		ImVec2 velocity;
		int cluster = 0;
	}; 

	AppLog mApplog;

	WallG::Graphics::Texture mUnitTexture;
	std::vector<Unit> mUnits;
};

