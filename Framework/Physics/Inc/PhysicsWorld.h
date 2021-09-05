#pragma once
#include "Particle.h" 

namespace WallG::Physics
{
	class PhysicsWorld
	{
	public:
		void Initialize();
		void Update(float deltaTime);
		void DebugDraw() const;
		//For simulation
		Particle* AddParticle();
	private:
		void AccumulateForce();
		void Integrate();
		void SatisfyConstraints();

		std::vector<std::unique_ptr<Particle>> mParticles;
	};
}