#pragma once
#include "Bone.h"

namespace WallG::Graphics
{
	struct  Skeleton
	{
		Bone* root = nullptr;
		std::vector<std::unique_ptr<Bone>> bones;
	};

	void DrawSkeleton(const Skeleton& skeleton, const std::vector<Math::Matrix4>& boneMatrices);
	void GetRootTransform(const Skeleton& skeleton, std::vector<Math::Matrix4>& toRootTransfrom);
	//[a b c 0]  right vector(x axis)
	//[d e f 0]  up vector (y axis)
	//[g h i 0]  look vector (z axis)
	//[x y z 1]  position
} 