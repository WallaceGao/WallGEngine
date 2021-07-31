#pragma once
#include "Common.h"

namespace WallG
{
	enum  ComponentId
	{
		Collider,
		Transform,
		Model,
		Animator,
		PlanetModel,
		Ship,
	};

	enum ServiceId
	{
		Render,
		Universe
	};
}

#define SET_TYPE_ID(id)\
	static uint32_t StaticGetTypeId() {return id;}\
	uint32_t GetTypeId() const override {return id;}