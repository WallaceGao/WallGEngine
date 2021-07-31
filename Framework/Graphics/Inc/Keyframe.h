#pragma once
#include "Common.h"

namespace WallG::Graphics
{
	template<class T>
	struct Keyframe
	{
		T Key;
		float time = 0.0f;
	};

	template <class T>
	using Keyframes = std::vector<Keyframe<T>>;
	
	using PositionKeys = Keyframes<Math::Vector3>;
	using RotationKeys = Keyframes<Math::Quaternion>;
	using ScaleKeys = Keyframes<Math::Vector3>;
}