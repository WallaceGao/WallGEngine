#pragma once

#include "Colors.h"

namespace WallG::Graphics
{
	struct Material 
	{
		Color ambient;		// 16
		Color diffuse;		// 16
		Color specular;		// 16
		float Power;		// 4
		float padding[3];	// 12
	};
}