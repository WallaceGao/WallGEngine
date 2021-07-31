#pragma once

#include "Common.h"

namespace WallG::Graphics
{
	struct Bone
	{
		std::string name;
		int index = -1;

		Bone* parent = nullptr;
		int parentIndex = -1;

		std::vector<Bone*> children;
		std::vector<int> childIndices;

		Math::Matrix4 toParentTransform; // postion for parent
		Math::Matrix4 offsetTransform; // postion for world?
	};
}