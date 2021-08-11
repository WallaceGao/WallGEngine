#pragma once
#include "WallG/Inc/ModelComponent.h"

namespace WallG
{
	class TransformComponent;

	class ShipComponent : public Component
	{
	public:
		SET_TYPE_ID(ComponentId::Ship);
		MEMPOOL_DECLARE;

		enum class State
		{
			Idle,
			Mine,
			Fly
		};

		ShipComponent() = default;

		void Initialize() override;
		void Terminate() override;
		void Update(float deltaTime) override;
		void DebugUI() override;

		void SetTargetPosition(Math::Vector3 position) { mTargetPosition = position; }
		Math::Vector3 GetTargetPostion() const { return mTargetPosition; }
		void SetSpeed(float speed) { mSpeed = speed; }
		float GetSpeed() const { return mSpeed; }
		void SetMineSpeed(float mineSpeed) { mMineSpeed= mineSpeed; }
		float GetMineSpeed() const { return mMineSpeed; }
		void SetCargoSize(int cargoSize) { mCargoSize = cargoSize; }
		int GetCargo() const { return mCargo; }
		bool GetIsMining() const { return mState == State::Mine; }


	protected:
		//class ShipAgent : public AI::Agent
		//{
		//
		//};
		//
		//ShipAgent mShipAgent;
		//
		//std::unique_ptr<AI::StateMachine<ShipAgent>> mStateMachine;

		void Idle(float deltaTime);
		void Mine(float deltaTime);
		void Fly(float deltaTime);

		TransformComponent* mTransformComponent;
		Math::Vector3 mTargetPosition = Math::Vector3::Zero;
		float mSpeed = 0.0f;
		float mMineSpeed = 0.0f;

		State mState = State::Idle;

		int mCargo = 0;
		int mCargoSize = 0;
	};
}