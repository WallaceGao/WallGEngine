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
	WallG::Math::Vector3 mPosition;
	WallG::Math::Vector3 mRotation;
	float mFPS = 0.0f;

	bool mShowAnimation = false;
	bool mShowSkeleton = false;

	WallG::GameWorld  mGameWorld;
	WallG::GameObjectHandle mGameObjectHandle;

	int mNextId = 0;
	float mNextPositionX = 0.0f;
};

