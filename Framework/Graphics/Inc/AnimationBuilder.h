#pragma once
#include "Animation.h"

namespace WallG::Graphics
{
	class AnimationBuilder
	{
	public:
		AnimationBuilder& SetTime(float time);
		AnimationBuilder& AdvanceTime(float time);

		AnimationBuilder& AddPositionKey(const Math::Vector3& position /*, Easing = Default */);
		AnimationBuilder& AddPositionKey(const Math::Vector3& position, const float time);
		AnimationBuilder& AddRotationKey(const Math::Quaternion& roatation);
		AnimationBuilder& AddRotationKey(const Math::Quaternion& roatation, const float time);
		AnimationBuilder& AddScaleKey(const Math::Vector3& scale);
		AnimationBuilder& AddScaleKey(const Math::Vector3& scale, const float time);

		AnimationBuilder& SetLooping(bool looping);

		Animation Get();
	private:
		
		Animation mTemp;
		float mTime = 0.0f;
		
	};
}