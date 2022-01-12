#pragma once
#include "Component.h"

namespace WallG
{
	//struct ParticleSetting
	//{
	//	std::string name;
	//	WallG::Math::Vector3 mPosition = WallG::Math::Vector3::Zero;
	//	float mStartSize = 0.0f;
	//	float mEndSize = 0.0f;
	//	float mMaxLifeTime = 0.0f;
	//	WallG::Math::Vector4 mStartColor = { 0.529411793f, 0.807843208f, 0.921568692f, 1.000000000f };
	//	WallG::Math::Vector4 mEndColor = { 0.529411793f, 0.807843208f, 0.921568692f, 1.000000000f };
	//};

	//class ParticleComponent final : public Component
	//{
	//public:
	//	SET_TYPE_ID(ComponentId::Particle);
	//	MEMPOOL_DECLARE;
	//
	//	ParticleComponent() = default;
	//
	//	void Initialize() override;
	//	void Terminate() override;
	//	void Update(float deltaTime) override;
	//	void DebugUI() override;
	//
	//	void AddNewParticle(ParticleSetting particleSetting);
	//
	//private:
	//	std::vector<ParticleSetting> mPartocleSettings;
	//	std::vector<WallG::Graphics::ParticleEmitter> mParticleEmitters;
	//};
}