#include "UniverseService.h"

#include "FactoryComponent.h"
#include "PlanetComponent.h"

using namespace WallG;
using namespace WallG::Graphics;
using namespace WallG::Input;
using namespace WallG::Math;

void UniverseService::Register(const PlanetComponent* planetComponent)
{
	const GameObject& planetObject = planetComponent->GetOwner();
	Entry& entry = mUniverseEntries.emplace_back();
	entry.planetComponent = planetComponent;
	entry.factoryComponent = planetObject.GetComponent<FactoryComponent>();
	entry.transformComponent = planetObject.GetComponent<TransformComponent>();
}

void UniverseService::Unregister(const PlanetComponent* planet)
{
	auto match = [&](const auto& entry) {return (entry.planetComponent == planet); };
	auto iter = std::find_if(mUniverseEntries.begin(), mUniverseEntries.end(), match);
	if (iter != mUniverseEntries.end())
	{
		mUniverseEntries.erase(iter);
	}
}

const GameObject* UniverseService::GetNearestPlanetWithResourceType(Vector3 position, MineralType desiredResource) const
{
	const GameObject* NearestPlanet = nullptr;
	float closestDistance = std::numeric_limits<float>::max();
	for (auto& entry : mUniverseEntries)
	{
		if (entry.planetComponent->GetResourceType() == desiredResource)
		{
			const float distanceToPlanet = Distance(position, entry.transformComponent->GetPosition());
			if (closestDistance > distanceToPlanet)
			{
				closestDistance = distanceToPlanet;
				NearestPlanet = &entry.planetComponent->GetOwner();

				// * - "dereference" convert pointer to type
				// & - "address of" convert type to pointer
			}
		}
	}
	return NearestPlanet;
}

const GameObject* UniverseService::GetNearestPlanetHaveFactory(Vector3 position) const
{
	const GameObject* NearestPlanet = nullptr;
	float closestDistance = std::numeric_limits<float>::max();
	for (auto& entry : mUniverseEntries)
	{
		if (entry.factoryComponent != nullptr)
		{
			const float distanceToPlanet = Distance(position, entry.transformComponent->GetPosition());
			if (closestDistance > distanceToPlanet)
			{
				closestDistance = distanceToPlanet;
				NearestPlanet = &entry.planetComponent->GetOwner();
			}
		}
	}
	return NearestPlanet;
}

const WallG::GameObject* UniverseService::GetPlanet(std::string name) const
{
	const GameObject* planet = nullptr;
	for (auto& entry : mUniverseEntries)
	{
		const GameObject& planetObject = entry.planetComponent->GetOwner();
		if (planetObject.GetName() == name)
		{
			planet = &planetObject;
			break;
		}
	}
	return planet;
}




