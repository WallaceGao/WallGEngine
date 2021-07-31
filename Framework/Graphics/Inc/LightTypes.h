#pragma once

#include "Colors.h"

namespace WallG::Graphics
{
	struct DirectionalLight
	{
		Math::Vector3 direction;
		float padding;
		Color ambient;		// 16
		Color diffuse;		// 16
		Color specular;		// 16
	};
}
