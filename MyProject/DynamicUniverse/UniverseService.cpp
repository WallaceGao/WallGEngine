#include "UniverseService.h"

#include "PlanetComponent.h"

using namespace WallG;
using namespace WallG::Graphics;
using namespace WallG::Input;
using namespace WallG::Math;

void UniverseService::Initialize()
{


}

void UniverseService::Terminate()
{

}

void UniverseService::Update(float deltaTime)
{



}


void UniverseService::DebugUI()
{

}

void UniverseService::Register(const PlanetComponent* planet)
{
	auto& gameObject = planet->GetOwner();

}

void UniverseService::Unregister(const PlanetComponent* planet)
{
}

GameObject* UniverseService::GetNearestPlanetWithResourceType(Vector3 position, MineralType desiredResource) const
{
	return nullptr;
}

GameObject* UniverseService::GetNearestPlanetHaveFactory(Vector3 position) const
{
	return nullptr;
}




