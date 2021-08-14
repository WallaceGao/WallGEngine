#pragma once
#include <WallG/Inc/WallG.h>

enum class MineralType
{
	None,
	Iron,
	Copper
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
	Factory 
};