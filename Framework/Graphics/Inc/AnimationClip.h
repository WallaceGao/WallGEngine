#pragma once
#include "Animation.h"

namespace WallG::Graphics
{
	struct AnimationClip
	{
		std::string name;
		float duration = 0.0f;
		float ticksPersecond = 0.0f;
		std::vector<std::unique_ptr<Animation>> boneAnimations;
	};
}

