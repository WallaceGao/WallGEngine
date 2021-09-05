#include "ShipComponent.h"

#include "DynamicUniverseTypes.h"
#include "FactoryService.h"
#include "PlanetComponent.h"
#include "UniverseService.h"
#include "ShipService.h"

using namespace WallG;
using namespace WallG::Math;

MEMPOOL_DEFINE(ShipComponent, 1000)

void ShipComponent::Initialize()
{
	mTransformComponent = GetOwner().GetComponent<TransformComponent>();
	//mStateMachine = std::make_unique<AI::StateMachine<ShipAgent>>(mShipAgent);
	
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
}

void ShipComponent::DebugUI()
{
	if (ImGui::CollapsingHeader("Ship", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::Text("Position: %f, %f, %f", mTransformComponent->GetPosition().x, mTransformComponent->GetPosition().y, mTransformComponent->GetPosition().z);
		ImGui::Text("Money: %f", mMoney);
		ImGui::Text("Cargo: %f", mCargo);

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


	}
}

void ShipComponent::Idle(float deltaTime)
{
	// In FSM - a state performs a task, then check transitions
	
	// Task - What should I do next?
	// What is the best mineral to mine
	const auto& world = GetOwner().GetWorld();
	//mining
	if (mCargo == 0)
	{
		const auto factoryService = world.GetService<FactoryService>();
		MineralType desiredResource = factoryService->GetMostDesiredResource();
		const auto universeService = world.GetService<UniverseService>();
		mTargetPlanet = universeService->GetNearestPlanetWithResourceType(mTransformComponent->GetPosition(), desiredResource)->GetHandle();
		mShipCargoType = desiredResource;
		// Transition - which state to go top
		mState = State::Fly;
	}
	//Selling
	else
	{
		//find factory
		const auto universeService = world.GetService<UniverseService>();
		mTargetPlanet = universeService->GetNearestPlanetHaveFactory(mTransformComponent->GetPosition())->GetHandle();

	}
}

void ShipComponent::Mine(float deltaTime)
{
	// Task - Do I have enough yet?
	GameObject* planet = GetOwner().GetWorld().GetGameObject(mTargetPlanet);
	ASSERT(planet != nullptr, "planet shouldn't be empty");
	TransformComponent* targetTaransform = planet->GetComponent<TransformComponent>();
	mTransformComponent->SetPosition(targetTaransform->GetPosition() + mDockOffset);

	// mining
	mCargo += mMineSpeed * deltaTime;

	if (mCargo >= mCargoSize)
	{
		mCargo = mCargoSize;
		mState = State::Idle;
	}

	mMineTimer += deltaTime * 10.0f;
	if (mShipType == ShipType::SmallShip)
	{
		SimpleDraw::AddRing(mTransformComponent->GetPosition(), 5.0f + sin(mMineTimer), Colors::Magenta);
	}
	else if (mShipType == ShipType::lagerShip)
	{
		SimpleDraw::AddRing(mTransformComponent->GetPosition(), 5.0f + sin(mMineTimer), Colors::Green);
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
		//if (Magnitude(posToPlanet) < planet->GetComponent<PlanetComponent>()->GetRadius())
		if (Magnitude(posToPlanet) < 15.0f)
		{
			// fly finish
			mState = State::Mine;
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


