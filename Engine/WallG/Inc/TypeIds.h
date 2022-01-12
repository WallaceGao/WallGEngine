#pragma once
#include "Common.h"

namespace WallG
{

	enum class ComponentId
	{
		Collider,
		Transform,
		Model,
		Animator,
		SkyBox,
		Material,
		Particle,
		Count
	};

	enum class ServiceId
	{
		Render,
		Camera,
		SkyBox,
		Count
	};
}

#define SET_TYPE_ID(id)\
	static uint32_t StaticGetTypeId() {return static_cast<uint32_t>(id);}\
	uint32_t GetTypeId() const override {return static_cast<uint32_t>(id);}
