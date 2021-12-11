#pragma once

#include "Common.h"

namespace WallG::Physics
{
	// Verlet Integration - velocity less, numerically stable
	struct Particle
	{
		Math::Vector3 position = Math::Vector3::Zero;
		Math::Vector3 lastPosition = Math::Vector3::Zero;
		Math::Vector3 acceleration = Math::Vector3::Zero;
		float radius = 1.0f;
		float invMass = 0.0f; // given a force f, acceleation force/mass
		float bounce = 1.0f; // 1 mean no energy
	
		void SetPosition(const Math::Vector3& pos)
		{
			position = pos;
			lastPosition = pos;
		}

		void SetVelocity(const Math::Vector3& velocity)
		{
			lastPosition = position - velocity;
		}
	};
}
