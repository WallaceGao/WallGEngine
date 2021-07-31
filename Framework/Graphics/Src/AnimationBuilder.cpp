#include "Precompiled.h"
#include "AnimationBuilder.h"

using namespace WallG::Graphics;

AnimationBuilder& AnimationBuilder::SetTime(float time)
{
	mTime = time;
	return *this;
}

AnimationBuilder& AnimationBuilder::AdvanceTime(float time)
{
	mTime += time;
	return *this;
}

AnimationBuilder& AnimationBuilder::AddPositionKey(const Math::Vector3& position)
{
	ASSERT(mTemp.mPositionKeys.empty() || mTemp.mPositionKeys.back().time < mTime, "The seting time is bigger than real time ");
	mTemp.mPositionKeys.push_back({ position,mTime });
	return *this;	
}

AnimationBuilder& WallG::Graphics::AnimationBuilder::AddPositionKey(const Math::Vector3& position, const float time)
{
	//ASSERT(mTemp.mPositionKeys.empty() || mTemp.mPositionKeys.back().time < mTime, "The seting time is bigger than real time ");
	mTemp.mPositionKeys.push_back({ position, time });
	return *this;
}

AnimationBuilder& AnimationBuilder::AddRotationKey(const Math::Quaternion& roatation)
{
	ASSERT(mTemp.mRotationKeys.empty() || mTemp.mRotationKeys.back().time < mTime, "The seting time is bigger than real time ");
	mTemp.mRotationKeys.push_back({roatation, mTime });
	return *this;
}

AnimationBuilder& WallG::Graphics::AnimationBuilder::AddRotationKey(const Math::Quaternion& roatation, const float time)
{
	//ASSERT(mTemp.mRotationKeys.empty() || mTemp.mRotationKeys.back().time < mTime, "The seting time is bigger than real time ");
	mTemp.mRotationKeys.push_back({ roatation, time });
	return *this;
}

AnimationBuilder& AnimationBuilder::AddScaleKey(const Math::Vector3& scale)
{
	ASSERT(mTemp.mScaleKeys.empty() || mTemp.mScaleKeys.back().time < mTime, "The seting time is bigger than real time ");
	mTemp.mScaleKeys.push_back({scale, mTime });
	return *this;
}

AnimationBuilder& WallG::Graphics::AnimationBuilder::AddScaleKey(const Math::Vector3& scale, const float time)
{
	//ASSERT(mTemp.mScaleKeys.empty() || mTemp.mScaleKeys.back().time < mTime, "The seting time is bigger than real time ");
	mTemp.mScaleKeys.push_back({ scale, time });
	return *this;
}

AnimationBuilder& AnimationBuilder::SetLooping(bool looping)
{
	mTemp.mLooping = looping;
	return *this;
}

Animation AnimationBuilder::Get()
{
	mTime = 0.0f;
	return std::move(mTemp);
}
