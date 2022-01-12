#pragma once
#include "Constraints.h"
#include "Particle.h" 


namespace WallG::Physics
{

	struct Tetrahedron
	{
		Particle* a, b, c, d;

		Math::Vector3 GetLook();
		Math::Vector3 GetRight();
		Math::Vector3 GetUp();
		Math::Vector3 GetPosition();

		Math::Matrix4 GetWorldMatrix();
	};

	class PhysicsWorld
	{
	public:
		struct Settings
		{
			Math::Vector3 gravity{ 0.0f, -9.8f, 0.0f };
			float timeStep = 1.0f / 60.0f;
			int iterations = 1;
		};

		void Initialize(Settings settings = {});
		void Update(float deltaTime);
		void DebugDraw() const;
		void DebugUI();
		//For simulation
		Particle* AddParticle();

		//use variat template argument
		template<class ConstraintType, class... Args>
		ConstraintType* AddConstraint(Args&&... args) // forwarding reference : it can be r reference and l reference 
		{
			auto& newConstraint = mConstraints.emplace_back(std::make_unique<ConstraintType>(std::forward<Args>(args)...));
			return static_cast<ConstraintType*>(newConstraint.get());
		}

		void Clear();

	private:
		void AccumulateForce();
		void Integrate();
		void SatisfyConstraints();

		std::vector<std::unique_ptr<Constraint>> mConstraints;
		std::vector<std::unique_ptr<Particle>> mParticles;
		
		Settings mSettings;
		float mTimer = 0.0f;
		bool mPause = false;
	};
}