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
}

void ShipComponent::DebugUI()
{

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
	auto shipToPlanet = targetTaransform->GetPosition() - mTransformComponent->GetPosition();
	auto distToPlanet = Magnitude(shipToPlanet);
	
	if (distToPlanet > 5.0f)
	{
		auto direction = shipToPlanet / distToPlanet;
		mTransformComponent->SetPosition(mTransformComponent->GetPosition() + direction * deltaTime * 50.0f); 
	}
	// mining
	mCargo += mMineSpeed * deltaTime;

	if (mCargo >= mCargoSize)
	{
		mCargo = mCargoSize;
		mState = State::Idle;
	}
}

void ShipComponent::Fly(float deltaTime)
{
	GameObject* planet = GetOwner().GetWorld().GetGameObject(mTargetPlanet);
	if (planet != nullptr)
	{
		TransformComponent* targetTaransform = planet->GetComponent<TransformComponent>();
		auto direction = Normalize(targetTaransform->GetPosition() - mTransformComponent->GetPosition());
		if (Magnitude(direction) < 5.0f)
		{
			//fly finish
			mState = State::Mine;
		}
		else
		{
			// keep flying
			mTransformComponent->SetPosition(mTransformComponent->GetPosition() + direction * deltaTime * mSpeed);
			mTransformComponent->SetRotation(Quaternion::RotationFromTo(Math::Vector3::ZAxis, direction));
		}
	}
	else
	{
		mState = State::Idle;
	}
}


