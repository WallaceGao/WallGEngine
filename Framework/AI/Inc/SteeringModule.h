#pragma once

#include "SteeringBehavior.h"

namespace WallG::AI
{
	class SteeringModule
	{
	public:
		SteeringModule(Agent& agent);

		template <class BehaviorType>
		BehaviorType* AddBehavior();

		Math::Vector2 Calculate();
	private:
		using Behaviors = std::vector<std::unique_ptr<SteeringBehavior>>;

		Agent& mAgent;
		Behaviors mBehaviors;
	};

	template <class BehaviorType>
	BehaviorType* SteeringModule::AddBehavior()
	{
		static_assert(std::is_base_of_v<SteeringBehavior, BehaviorType>, "BehaviorType is not a child class of AI::SteeringBehavior");
		auto& newBehaviour = mBehaviors.emplace_back(std::make_unique<BehaviorType>());
		return static_cast<BehaviorType*>(newBehaviour.get());
	}
}
