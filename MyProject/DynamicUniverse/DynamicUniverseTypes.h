#pragma once
#include <WallG/Inc/WallG.h>

// skybox : not really down
// camera : not really down
// planet model : down
// different ship type : down
// add 经济系统

// 2d game : down

// wirte down what I want to talk 

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
	Factory
};

enum class DUServiceId
{
	Universe = static_cast<uint32_t>(WallG::ServiceId::Count),
	Factory,
	Ship
};
