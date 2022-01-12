#pragma once
#include <WallG/Inc/WallG.h>

struct ParticleSetting
{
	WallG::Math::Vector3 mPosition = WallG::Math::Vector3::Zero;

	float mStartSize = 0.25f;
	float mEndSize = 0.01f;
	float mMaxLifeTime = 1.5f;

	WallG::Math::Vector4 mStartColor = { 0.529411793f, 0.807843208f, 0.921568692f, 1.000000000f };
	WallG::Math::Vector4 mEndColor = { 0.529411793f, 0.807843208f, 0.921568692f, 1.000000000f };
};

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

	WallG::GameWorld  mGameWorld;
	WallG::GameObjectHandle mGameObjectHandle;
	WallG::Graphics::ParticleEmitter mParticleEmitter01;
	WallG::Graphics::ParticleEmitter mParticleEmitter02;

	ParticleSetting mParticleSetting01;
	ParticleSetting mParticleSetting02;
};

