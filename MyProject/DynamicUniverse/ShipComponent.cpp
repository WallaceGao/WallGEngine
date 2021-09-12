#include "ShipComponent.h"

#include "DynamicUniverseTypes.h"
#include "FactoryComponent.h"
#include "FactoryService.h"
#include "PlanetComponent.h"
#include "UniverseService.h"
#include "ShipService.h"
#include <ImGui/Inc/ImPlot.h>

using namespace WallG;
using namespace WallG::Math;

MEMPOOL_DEFINE(ShipComponent, 1000)

void ShipComponent::Initialize()
{
	mTransformComponent = GetOwner().GetComponent<TransformComponent>();
	//mStateMachine = std::make_unique<AI::StateMachine<ShipAgent>>(mShipAgent);
	//int mMineSpeedLevel = 1;
	//int mCargoLevel = 1;
	//int mSpeedLevel = 1;
	auto shipService = GetOwner().GetWorld().GetService<ShipService>();
	shipService->Register(this);
}

void ShipComponent::Terminate()
{
	auto shipService = GetOwner().GetWorld().GetService<ShipService>();
	shipService->Unregister(this);
	mTransformComponent = nullptr;
}

void ShipComponent::Update(float deltaTime)
{
	//// Synchronize - update agent information
	//mShipAgent.position = mTransformComponent->GetPosition();
	//
	//// Make decision
	//mStateMachine->Update(deltaTime);
	//mSteering->Calucate();
	//
	//// Feedback
	//mTransformComponent->SetPosition();

	switch (mState)
	{
	case State::Idle:
		Idle(deltaTime);
		break;
	case State::Mine:
		Mine(deltaTime);
		break;
	case State::Fly:
		Fly(deltaTime);
		break;
	case State::Sell:
		Sell(deltaTime);
		break;
	case State::Upgrade:
		Upgrade(deltaTime);
		break;
	}

	for (size_t i = 0; i + 1 < mTailPositions.size(); ++i)
	{
		if (mShipType == ShipType::SmallShip)
		{
			SimpleDraw::AddLine(mTailPositions[i], mTailPositions[i + 1], Colors::Magenta);
		}
		else if(mShipType == ShipType::lagerShip)
		{
			SimpleDraw::AddLine(mTailPositions[i], mTailPositions[i + 1], Colors::Green);
		}
	}

	static float waitTime = 0.0f;
	waitTime -= deltaTime;
	if (waitTime <= 0.0f)
	{
		waitTime += 0.5f;
		float time = Core::TimeUtil::GetTime();
		mMoneyPoints.push_back({ time, mMoney });
		mCargoPoints.push_back({time, mCargo});
	}
	if (mMoneyPoints.size() > 20000)
	{
		mMoneyPoints.erase(mMoneyPoints.begin());
	}
}

void ShipComponent::DebugUI()
{
	if (ImGui::CollapsingHeader("Ship", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::Text("Position: %f, %f, %f", mTransformComponent->GetPosition().x, mTransformComponent->GetPosition().y, mTransformComponent->GetPosition().z);

		if (mState == State::Idle)
		{
			ImGui::Text("State: Idle");
		}
		else if (mState == State::Fly)
		{
			ImGui::Text("State: Fly");

			GameObject* planet = GetOwner().GetWorld().GetGameObject(mTargetPlanet);
			if (planet != nullptr)
			{
				ImGui::Text("  Target: %s", planet->GetName().c_str());
			}
		}
		else if (mState == State::Mine)
		{
			ImGui::Text("State: Mine");
		}
		else if(mState == State::Sell)
		{
			ImGui::Text("State: Sell");
		}
		else
		{
			ImGui::Text("State: Upgrade");
		}

		if (mShipType == ShipType::UnKnow)
		{
			ImGui::Text("Ship are not assign");
		}
		else if(mShipType == ShipType::SmallShip)
		{
			ImGui::Text("ShipType: Small Ship");
		}
		else 
		{
			ImGui::Text("ShipType: lager Ship");
		}

		if (ImPlot::BeginPlot("MoneyInCome", "X", "Y"))
		{
			ImPlot::PlotLine("Ship Money", &mMoneyPoints[0].x, &mMoneyPoints[0].y, static_cast<int>(mMoneyPoints.size()),0, sizeof(Vector2));
			ImPlot::PlotLine("Ship Cargo", &mCargoPoints[0].x, &mCargoPoints[0].y, static_cast<int>(mCargoPoints.size()), 0, sizeof(Vector2));
			ImPlot::EndPlot();
		}
	}
}

void ShipComponent::Idle(float deltaTime)
{
	// In FSM - a state performs a task, then check transitions
	
	// Task - What should I do next?
	// What is the best mineral to mine
	const auto& world = GetOwner().GetWorld();

	if (mMoney > 2000.0f)
	{
		int randomUpGrade = rand()%3 + 0;
		switch (randomUpGrade)
		{
		case 0:
			mUpGradeType = ItemType::Engine;
			break;
		case 1:
			mUpGradeType = ItemType::MineTool;
			break;
		case 2:
			mUpGradeType = ItemType::Cargo;
			break;
		default:
			break;
		}
		const auto universeService = world.GetService<UniverseService>();
		mTargetPlanet = universeService->GetNearestPlanetHaveFactory(mTransformComponent->GetPosition())->GetHandle();
		mState = State::Fly;
		mFState = FlySate::FlyToUpgrade;
	}
	//mining
	else if (mCargo == 0)
	{
		const auto factoryService = world.GetService<FactoryService>();
		MineralType desiredResource = factoryService->GetMostDesiredResource();
		const auto universeService = world.GetService<UniverseService>();
		mTargetPlanet = universeService->GetNearestPlanetWithResourceType(mTransformComponent->GetPosition(), desiredResource)->GetHandle();
		mShipCargoType = desiredResource;
		// Transition - which state to go top
		mState = State::Fly;
		mFState = FlySate::FlyToMine;
	}
	//Selling
	else
	{
		//find factory
		const auto universeService = world.GetService<UniverseService>();
		mTargetPlanet = universeService->GetNearestPlanetHaveFactory(mTransformComponent->GetPosition())->GetHandle();
		mState = State::Fly;
		mFState = FlySate::FlyToSell;
	}
}

void ShipComponent::Fly(float deltaTime)
{
	GameObject* planet = GetOwner().GetWorld().GetGameObject(mTargetPlanet);
	if (planet != nullptr)
	{
		TransformComponent* targetTransform = planet->GetComponent<TransformComponent>();
		auto posToPlanet = targetTransform->GetPosition() - mTransformComponent->GetPosition();
		auto direction = Normalize(posToPlanet);
		if (Magnitude(posToPlanet) < planet->GetComponent<PlanetComponent>()->GetPlanetScale().x * 1.5f)
		{
			// fly finish
			if (mFState == FlySate::FlyToMine)
			{
				
				mState = State::Mine;
			}
			else if(mFState == FlySate::FlyToSell)
			{
				mState = State::Sell;
			}
			else if (mFState == FlySate::FlyToUpgrade)
			{
				mState = State::Upgrade;
			}
			mDockOffset = mTransformComponent->GetPosition() - targetTransform->GetPosition();
			mTailPositions.clear();
		}
		else
		{
			// keep flying
			mTransformComponent->SetPosition(mTransformComponent->GetPosition() + direction * deltaTime * mSpeed);
			mTransformComponent->SetRotation(Quaternion::RotationFromTo(Math::Vector3::ZAxis, direction));
			SimpleDraw::AddLine(mTransformComponent->GetPosition(), mTransformComponent->GetPosition() + direction * 10.0f, Colors::Green);
		}
	}
	else
	{
		mFState = FlySate::Idle;
		mState = State::Idle;
		mTailPositions.clear();
	}

	mNextTailTime -= deltaTime;
	if (mNextTailTime <= 0.0f)
	{
		mNextTailTime += 0.5f;
		mTailPositions.push_back(mTransformComponent->GetPosition());
		if (mTailPositions.size() > 20)
			mTailPositions.erase(mTailPositions.begin());
	}
}

void ShipComponent::Mine(float deltaTime)
{
	GameObject* planet = GetOwner().GetWorld().GetGameObject(mTargetPlanet);
	ASSERT(planet != nullptr, "planet shouldn't be empty");
	TransformComponent* targetTaransform = planet->GetComponent<TransformComponent>();
	mTransformComponent->SetPosition(targetTaransform->GetPosition() + mDockOffset);

	// mining
	mCargo += mMineSpeed * deltaTime;

	// Task - Do I have enough yet?
	if (mCargo >= mCargoSize)
	{
		mCargo = mCargoSize;
		mState = State::Idle;
	}

	mMineTimer += deltaTime * mMineSpeed;

	SimpleDraw::AddRing(mTransformComponent->GetPosition(), 5.0f + sin(mMineTimer), Colors::Magenta);

}

void ShipComponent::Sell(float deltaTime)
{
	GameObject* planet = GetOwner().GetWorld().GetGameObject(mTargetPlanet);
	ASSERT(planet != nullptr, "planet shouldn't be empty");
	TransformComponent* targetTaransform = planet->GetComponent<TransformComponent>();
	mTransformComponent->SetPosition(targetTaransform->GetPosition() + mDockOffset);

	// Selling

	if (mWaitTime > 30.0f)
	{
		mMoney += planet->GetComponent<FactoryComponent>()->BuyMinral(mShipCargoType, mCargo);
		mShipCargoType = MineralType::None;
		mState = State::Idle;
		mCargo = 0.0f;
		mWaitTime = 0.0f;
	}

	mWaitTime += deltaTime * mSellSpeed;

}

void ShipComponent::Upgrade(float deltaTime)
{
	GameObject* planet = GetOwner().GetWorld().GetGameObject(mTargetPlanet);
	ASSERT(planet != nullptr, "planet shouldn't be empty");
	TransformComponent* targetTaransform = planet->GetComponent<TransformComponent>();
	mTransformComponent->SetPosition(targetTaransform->GetPosition() + mDockOffset);

	//UpGrade
	// every update make level +1
	if (mUpGradeType == ItemType::MineTool && mWaitTime > 30.0f)
	{
		mUpGradeType = ItemType::None;
		mState = State::Idle;
		mMineSpeed += 1;
		mWaitTime = 0.0f;
		//mMineSpeedLevel += 1;
		mMoney -= planet->GetComponent<FactoryComponent>()->SellItem(mUpGradeType);
	}
	else if (mUpGradeType == ItemType::Engine && mWaitTime > 30.0f)
	{
		mUpGradeType = ItemType::None;
		mState = State::Idle;
		mSpeed += 50;
		mWaitTime = 0.0f;
		//mMineSpeedLevel += 1;
		mMoney -= planet->GetComponent<FactoryComponent>()->SellItem(mUpGradeType);
	}
	else if (mUpGradeType == ItemType::Cargo && mWaitTime > 30.0f)
	{
		mUpGradeType = ItemType::None;
		mState = State::Idle;
		mCargoSize += 10;
		mWaitTime = 0.0f;
		//mMineSpeedLevel += 1;
		mMoney -= planet->GetComponent<FactoryComponent>()->SellItem(mUpGradeType);
	}

	mWaitTime += deltaTime * mUpGradeSpeed;
	//std::sort(mItemsLevel, mItemsLevel);
}


