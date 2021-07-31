#pragma once
#include "Skeleton.h"
#include "AnimationClip.h"

namespace WallG::Graphics
{
	void GetAnimationTransForm(const Skeleton& skeleton, const AnimationClip& clip , float time, std::vector<Math::Matrix4>& toRootTransfrom);
}