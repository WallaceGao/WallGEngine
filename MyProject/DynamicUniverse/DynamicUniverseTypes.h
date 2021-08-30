#pragma once
#include <WallG/Inc/WallG.h>

// skybox
// camera
// planet model
// different ship type

// 2d game

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