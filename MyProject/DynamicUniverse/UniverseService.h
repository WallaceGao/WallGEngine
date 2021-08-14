#pragma once

#include "DynamicUniverseTypes.h"
#include <WallG/Inc/WallG.h>

class PlanetComponent;

class UniverseService final : public WallG::Service
{
public:
	SET_TYPE_ID(DUServiceId::Universe)

	void Initialize() override;
	void Terminate() override;
	void Update(float deltaTime) override;
	void DebugUI() override;

	void Register(const PlanetComponent* planet );
	void Unregister(const PlanetComponent* planet);

	WallG::GameObject* GetNearestPlanetWithResourceType(WallG::Math::Vector3 position, MineralType desiredResource) const;
	WallG::GameObject* GetNearestPlanetHaveFactory(WallG::Math::Vector3 position) const;

private:
	struct Entry
	{
		const PlanetComponent* planetComponent;
		const WallG::TransformComponent* transformComponent;
	};
	
};
