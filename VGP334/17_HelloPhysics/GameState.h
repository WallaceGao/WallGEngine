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

	WallG::Graphics::Camera mCamera;

	WallG::Physics::PhysicsWorld  mPhysicsWorld;

	float mFPS = 0.0f;
};

