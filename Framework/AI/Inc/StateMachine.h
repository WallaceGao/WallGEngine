#pragma once
#include "State.h"


namespace WallG::AI
{
	template <class AgentType>
	class StateMachine
	{
	public:
		using StateType = State<AgentType>;

		StateMachine(AgentType& agent);

		template <class NewStateType>
		void AddState();

		void Update(float deltaTime);
		void ChangeState(const std::string& stateName);

	private:
		using StateMap = std::unordered_map<std::string, std::unique_ptr<StateType>>;

		AgentType& mAgent;
		StateType* mCurrentState = nullptr;
		StateMap mStates;
	};

	template <class AgentType>
	StateMachine<AgentType>::StateMachine(AgentType& agent)
		: mAgent(agent)
	{}

	template <class AgentType>
	template <class NewStateType>
	void StateMachine<AgentType>::AddState()
	{
		static_assert(std::is_base_of<StateType, NewStateType>::value, "[StateMachine] Can only add state for AgentType.");
		auto [iter, success] = mStates.emplace(NewStateType::GetName(), std::make_unique<NewStateType>());
		if (!mCurrentState)//Add a state become first state
		{
			mCurrentState = iter->second.get();
		}
	}

	template <class AgentType>
	void StateMachine<AgentType>::Update(float deltaTime)
	{
		mCurrentState->Update(mAgent, deltaTime);
	}

	template <class AgentType>
	void StateMachine<AgentType>::ChangeState(const std::string& stateName)
	{
		auto iter = mStates.find(stateName);
		if (iter == mStates.end())
			return;

		mCurrentState->Exit(mAgent);
		mCurrentState = iter->second.get();
		mCurrentState->Enter(mAgent);
	}
}