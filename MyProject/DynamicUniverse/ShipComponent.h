#pragma once

#include "DynamicUniverseTypes.h"
#include <WallG/Inc/WallG.h>

class ShipComponent : public WallG::Component
{
public:
	SET_TYPE_ID(DUComponentId::Ship);
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

	void SetSpeed(float speed) { mSpeed = speed; }
	float GetSpeed() const { return mSpeed; }
	void SetMineSpeed(float mineSpeed) { mMineSpeed= mineSpeed; }
	float GetMineSpeed() const { return mMineSpeed; }
	void SetCargoSize(float cargoSize) { mCargoSize = cargoSize; }
	float GetCargo() const { return mCargo; }
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

	WallG::TransformComponent* mTransformComponent;
	WallG::GameObjectHandle mTargetPlanet;
	float mSpeed = 0.0f;
	float mMineSpeed = 0.0f;

	State mState = State::Idle;
	MineralType mShipCargoType = MineralType::None;
	float mCargo = 0;
	float mCargoSize = 0;
};
