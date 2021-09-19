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
		Fly,
		Sell,
		Upgrade
	};

	enum class FlySate
	{
		Idle,
		FlyToMine,
		FlyToSell,
		FlyToUpgrade
	};

	struct ItemLevel
	{
		ItemType item;
		int level;
	};

	ShipComponent() = default;

	void Initialize() override;
	void Terminate() override;
	void Update(float deltaTime) override;
	void DebugUI() override;

	void SetSpeed(float speed) { mSpeed = speed; }
	const float GetSpeed() const { return mSpeed; }
	void SetMineSpeed(float mineSpeed) { mMineSpeed= mineSpeed; }
	const float GetMineSpeed() const { return mMineSpeed; }
	void SetCargoSize(float cargoSize) { mCargoSize = cargoSize; }
	const float GetCargo() const { return mCargo; }
	void SetMoney(float money) { mMoney = money; }
	const float GetMoney() const { return mMoney; }
	void SetShipType(int shipType) { mShipType = ShipType(shipType); }
	const ShipType GetShipType() { return mShipType; }
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
	void Sell(float deltaTime);
	void Upgrade(float deltaTime);

	WallG::TransformComponent* mTransformComponent;
	WallG::GameObjectHandle mTargetPlanet;

	State mState = State::Idle;
	FlySate mFState = FlySate::Idle;
	MineralType mShipCargoType = MineralType::None;
	ShipType mShipType = ShipType::UnKnow;
	ItemType mUpGradeType = ItemType::None;
	float mSpeed = 0.0f;
	float mMaxSpeed = 0.0f;
	float mMineSpeed = 0.0f;
	float mMaxMineSpeed = 0.0f;
	float mMineTime = 0.0f;
	float mCargo = 0;
	float mCargoSize = 0;
	float mMaxCargoSize = 0;
	float mMoney = 0;
	float mWaitTime = 0;
	float mTimePassed = 0.0f;
	float mSellSpeed = 2.0f;
	float mUpGradeSpeed = 10.0f;
	float mUpSpeedPrice = 1500.0f;
	float mUpMinePrice = 1500.0f;
	float mUpCargoPrice = 1500.0f;
	// Mining params
	WallG::Math::Vector3 mDockOffset;
	float mMineTimer = 0.0f;

	WallG::Math::Vector3 mCurrentVelocity = WallG::Math::Vector3::Zero;
	std::vector<WallG::Math::Vector3> mTailPositions;
	float mNextTailTime = 0.0f;
	float mHackHeight = 0.0f;

	std::vector<ItemType> mItemsLevel;

	std::vector<WallG::Math::Vector2> mMoneyPoints;
	std::vector<WallG::Math::Vector2> mCargoPoints;
};
