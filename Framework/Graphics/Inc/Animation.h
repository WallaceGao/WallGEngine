#pragma once

#include "Keyframe.h"

namespace WallG::Graphics
{
	enum class EasingTimes
	{
		Nomal,EaseInQuad, EaseOutQuad
	};

	class Animation
	{
	public:
		Math::Vector3 GetPosition(float time) const;
		Math::Quaternion GetRotation(float time) const;
		Math::Vector3 GetScale(float time) const;

		float GetDuration() const;

		void SetAnimationLoop(bool isLoop) { mLooping = isLoop; }
		bool IsLooping() const { return mLooping; }

	private:
		friend class AnimationBuilder;
		friend class AnimationIO;

		PositionKeys mPositionKeys;
		RotationKeys mRotationKeys;
		ScaleKeys mScaleKeys;
		bool mLooping = false;
	};
}