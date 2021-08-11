#include "ShipComponent.h"

#include "WallG/Inc/GameWorld.h"
#include "WallG/Inc/TransformComponent.h"

using namespace WallG;
using namespace WallG::Math;

void WallG::ShipComponent::Initialize()
{
	mTransformComponent = GetOwner().GetComponent<TransformComponent>();

	//mStateMachine = std::make_unique<AI::StateMachine<ShipAgent>>(mShipAgent);
}

void WallG::ShipComponent::Terminate()
{
	mTransformComponent = nullptr;
	mSpeed = 0;
}

void WallG::ShipComponent::Update(float deltaTime)
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

void WallG::ShipComponent::DebugUI()
{

}

void WallG::ShipComponent::Idle(float deltaTime)
{
	//// In FSM - a state performs a task, then check transitions
	//
	//// Task - What should I do next?
	//// What is the best mineral to mine
	//const auto& world = GetOwner().GetWorld();
	//const auto factoryService = world.GetService<FactoryService>();
	//MineralType desiredResource = factoryService->GetMostDesiredResource();
	//
	//const auto universeService = world.GetService<UniverseService>();
	//mTargetPlanet = universeService->GetNearestPlanetWithResourceType(position, desiredResource);
	//
	//// Transition - which state to go top
	//mState = State::Fly;
}

void WallG::ShipComponent::Mine(float deltaTime)
{
	// Task - Do I have enough yet?

	// Transition - Where do I deliver my minerals

	// what type of item I mining with
	if (mCargoSize > mCargo)
	{
		// mining
	}
}

void WallG::ShipComponent::Fly(float deltaTime)
{
	//auto targetPosition = mTargetPlanet->GetComponent<TransformComponent>()->GetPosition();
	//
	//
	////fly begin
	//auto direction = targetPosition - mTransformComponent->GetPosition();
	//if (Magnitude(direction) < 5.0f)
	//{
	//	//fly finish
	//	mState = State::Idle;
	//}
	//mTransformComponent->SetPosition(mTransformComponent->GetPosition() + Normalize(direction) * deltaTime * 50.0f);
}


