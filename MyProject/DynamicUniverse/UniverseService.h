#pragma once

#include "DynamicUniverseTypes.h"
#include <WallG/Inc/WallG.h>

class FactoryComponent;
class PlanetComponent;

class UniverseService final : public WallG::Service
{
public:
	SET_TYPE_ID(DUServiceId::Universe)

	void Register(const PlanetComponent* planet );
	void Unregister(const PlanetComponent* planet);

	const WallG::GameObject* GetNearestPlanetWithResourceType(WallG::Math::Vector3 position, MineralType desiredResource) const;
	const WallG::GameObject* GetNearestPlanetHaveFactory(WallG::Math::Vector3 position) const;
	const WallG::GameObject* GetPlanet(std::string name) const;
	const PlanetComponent* GetRandomHomePlanet() const;

private:
	struct Entry
	{
		const PlanetComponent* planetComponent;
		const FactoryComponent* factoryComponent;
		const WallG::TransformComponent* transformComponent;
	};
	
	std::vector<Entry> mUniverseEntries;
};
