#include "Precompiled.h"
#include "Animation.h"

using namespace WallG::Graphics;
using namespace WallG::Math;

namespace
{
	float GetPercentTime(const float time, const float keyTime1, const float keyTime2)
	{
		float tempTime = (time - keyTime1) / (keyTime2 - keyTime1);
		ASSERT(1 >= tempTime && tempTime >= 0, "the tempTime is not right");
		return tempTime;
	}
}


Vector3 Animation::GetPosition(float time) const
{
	Vector3 temp;
	float percentTime =0.0f;
	float tempTime = time;


	if (time < mPositionKeys.front().time)
	{
		return mPositionKeys.front().Key;
	}
	else if (time < mPositionKeys.back().time || mLooping)
	{
		if (mLooping && time >= mPositionKeys.back().time)
		{
			const float duration = GetDuration();
			if (duration <= 0.0f)
				return mPositionKeys.back().Key;

			while (tempTime > duration)
			{
				tempTime -= duration;
			}
		}
		for (size_t i = 1; i < mPositionKeys.size(); i++)
		{
			if (tempTime > mPositionKeys[i-1].time && tempTime <= mPositionKeys[i].time)
			{
				percentTime = GetPercentTime(tempTime, mPositionKeys[i - 1].time, mPositionKeys[i].time);
				temp = Lerp(mPositionKeys[i-1].Key, mPositionKeys[i].Key, percentTime);
				break;
			}
		}
	}
	else if (time > mPositionKeys.back().time)
	{
		return mPositionKeys.back().Key;
	}


	return temp;
}

Quaternion Animation::GetRotation(float time) const
{
	Quaternion temp;
	float percentTime = 0.0f;
	float tempTime = time;

	if (mRotationKeys.empty())
	{
		return Quaternion::Identity;
	}


	if (time < mRotationKeys.front().time)
	{
		return mRotationKeys.front().Key;
	}
	else if (time < mRotationKeys.back().time || mLooping)
	{
		if (mLooping && time >= mRotationKeys.back().time)
		{
			const float duration = GetDuration();
			if (duration <= 0.0f)
				return mRotationKeys.back().Key;

			while (tempTime > duration)
			{
				tempTime -= duration;
			}
		}
		for (size_t i = 1; i < mRotationKeys.size(); i++)
		{
			if (tempTime > mRotationKeys[i - 1].time && tempTime <= mRotationKeys[i].time)
			{
				percentTime = GetPercentTime(tempTime, mRotationKeys[i - 1].time, mRotationKeys[i].time);
				temp = Slerp(mRotationKeys[i - 1].Key, mRotationKeys[i].Key, percentTime);
				break;
			}
		}
	}
	else if (time > mRotationKeys.back().time)
	{
		return mRotationKeys.back().Key;
	}

	return temp;
}

Vector3 Animation::GetScale(float time) const
{
	Vector3 temp;
	float percentTime = 0.0f;
	float tempTime = time;

	if (mScaleKeys.empty())
	{
		return {1,1,1};
	}

	else if (time < mScaleKeys.front().time)
	{
		return mScaleKeys.front().Key;
	}
	if (time < mScaleKeys.back().time || mLooping)
	{
		if (mLooping && time >= mScaleKeys.back().time)
		{
			const float duration = GetDuration();
			if (duration <= 0.0f)
				return mScaleKeys.back().Key;

			while (tempTime > duration)
			{
				tempTime -= duration;
			}
		}
		for (size_t i = 1; i < mScaleKeys.size(); i++)
		{
			if (tempTime > mScaleKeys[i - 1].time && tempTime <= mScaleKeys[i].time)
			{
				percentTime = GetPercentTime(tempTime, mScaleKeys[i - 1].time, mScaleKeys[i].time);
				temp = Lerp(mScaleKeys[i - 1].Key, mScaleKeys[i].Key, percentTime);
				return temp;
			}
		}
	}
	else if(time > mScaleKeys.back().time)
	{
		return mScaleKeys.back().Key;
	}
}

float Animation::GetDuration() const
{
	if (mScaleKeys.empty() || mRotationKeys.empty() || mPositionKeys.empty())
		return 0.0f;

	return Math::Max(mScaleKeys.back().time, Math::Max(mRotationKeys.back().time, mPositionKeys.back().time));
}


