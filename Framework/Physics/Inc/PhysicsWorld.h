#pragma once
#include "Particle.h" 

namespace WallG::Physics
{
	class PhysicsWorld
	{
	public:
		struct Settings
		{
			Math::Vector3 gravity{ 0.0f, -9.8f, 0.0f };
			float timeStep = 1.0f / 60.0f;
		};

		void Initialize(Settings settings);
		void Update(float deltaTime);
		void DebugDraw() const;
		//For simulation
		Particle* AddParticle();
	private:
		void AccumulateForce();
		void Integrate();
		void SatisfyConstraints();

		std::vector<std::unique_ptr<Particle>> mParticles;
		
		Settings mSettings;
		float mTimer = 0.0f;
	};
}