#pragma once
#include <WallG/Inc/WallG.h>

enum class MineralType
{
	None,
	Iron,
	Copper
};

enum class ItemType
{
	None,
	Engine,
	Cargo,
	MineTool,
};

enum class ShipType
{
	UnKnow,
	SmallShip,
	lagerShip
};

enum class DUComponentId
{
	Planet = static_cast<uint32_t>(WallG::ComponentId::Count),
	Ship,
	Factory,
	Resources
};

enum class DUServiceId
{
	Universe = static_cast<uint32_t>(WallG::ServiceId::Count),
	Factory,
	Ship,
	Economic
};
