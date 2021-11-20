#pragma once

#include "Common.h"

namespace WallG::AI
{
	class Agent;

	class SteeringBehavior
	{
	public:
		SteeringBehavior() = default;
		virtual ~SteeringBehavior() = default;
		virtual Math::Vector2 Calculate(Agent& agent) = 0;
		void SetActive(bool active) { mActive = active; }
		bool IsActive() const { return mActive; }
		bool ShowDebug() const { return mShowDebug; }

	private:
		bool mActive = false;
		bool mShowDebug = false;
	};
}
