#pragma once
#include "Precompiled.h"
#include "AnimationIO.h"

using namespace WallG::Graphics;

void AnimationIO::Write(FILE* file, const std::vector<std::unique_ptr<AnimationClip>>& animSet)
{
	// Save animSet.size() first
	fprintf_s(file, "animSetSize: %d\n", static_cast<uint32_t>(animSet.size()));
	for (auto& animationClip : animSet)
	{
		//std::string name;
		fprintf_s(file, "Name: %s\n", animationClip->name.c_str());
		//float duration = 0.0f;
		fprintf_s(file, "Duration: %f\n", animationClip->duration);
		//float ticksPersecond = 0.0f;
		fprintf_s(file, "TicksPersecond: %f\n", animationClip->ticksPersecond);
		//std::vector<std::unique_ptr<Animation>> boneAnimations;
		fprintf_s(file, "BoneAnimationSize: %d\n", static_cast<uint32_t>(animationClip->boneAnimations.size()));
		for (const auto& boneAnimation : animationClip->boneAnimations)
		{	
			if (boneAnimation)
			{
				fprintf_s(file, "Animation:\n");
				WriteAnimation(file, *boneAnimation);
			}
			else
			{
				fprintf(file, "[Empty]\n");
			}
		}
	}
}

void AnimationIO::Read(FILE* file, std::vector<std::unique_ptr<AnimationClip>>& animSet)
{
	uint32_t animSize;
	fscanf_s(file, "animSetSize: %d\n", &animSize);
	animSet.resize(animSize);
	for (auto& animationClip : animSet)
	{
		animationClip = std::make_unique<AnimationClip>();
		char name[50];
		float duration = 0;
		float ticksPersecond = 0;
		uint32_t boneAnimSize = 0;


		//           [____]
		//           [____]
		//           [____]
		//           [____]
		//           [____]
		//           [____]
		//           [____]
		// 0x00ABCE  [____]
		// 0x00ABCD  [____]
		// ......
		// ......
		// ......
		// ......
		// 0x00FFB0  [__0x00ABCD__] (name)

		//std::string name;
		fscanf_s(file, "Name: %s\n", name, static_cast<uint32_t>(sizeof(name)));
		animationClip->name = name;
		//float duration = 0.0f;
		fscanf_s(file, "Duration: %f\n", &duration);
		animationClip->duration = duration;
		//float ticksPersecond = 0.0f;
		fscanf_s(file, "TicksPersecond: %f\n", &ticksPersecond);
		animationClip->ticksPersecond = ticksPersecond;
		//std::vector<std::unique_ptr<Animation>> boneAnimations;
		fscanf_s(file, "BoneAnimationSize: %d\n", &boneAnimSize);
		animationClip->boneAnimations.resize(boneAnimSize);
		for (auto& boneAnimation : animationClip->boneAnimations)
		{
			fscanf_s(file, "%s\n", name, static_cast<uint32_t>(sizeof(name)));
			if (std::string(name) == "Animation:")
			{
				//Create the data with unique so when you read in you assign all the values. 
				boneAnimation = std::make_unique<Animation>();
				ReadAnimation(file, *boneAnimation);
			}
		}
	}
}

void WallG::Graphics::AnimationIO::WriteAnimation(FILE* file, const Animation& animation)
{
		fprintf_s(file, "PositionSize: %d\n", static_cast<uint32_t>(animation.mPositionKeys.size()));
		for (uint32_t i = 0; i < animation.mPositionKeys.size(); ++i)
		{
			fprintf_s(file, "%f %f %f %f\n",
				animation.mPositionKeys[i].Key.x,
				animation.mPositionKeys[i].Key.y,
				animation.mPositionKeys[i].Key.z,
				animation.mPositionKeys[i].time);
		}

		fprintf_s(file, "RotationSize: %d\n", static_cast<uint32_t>(animation.mRotationKeys.size()));
		for (uint32_t i = 0; i < animation.mRotationKeys.size(); ++i)
		{
			fprintf_s(file, "%f %f %f %f %f\n",
				animation.mRotationKeys[i].Key.w,
				animation.mRotationKeys[i].Key.x,
				animation.mRotationKeys[i].Key.y,
				animation.mRotationKeys[i].Key.z,
				animation.mRotationKeys[i].time);
		}

		fprintf_s(file, "ScaleSize: %d\n", static_cast<uint32_t>(animation.mScaleKeys.size()));
		for (uint32_t i = 0; i < animation.mScaleKeys.size(); ++i)
		{
			fprintf_s(file, "%f %f %f %f\n",
				animation.mScaleKeys[i].Key.x,
				animation.mScaleKeys[i].Key.y,
				animation.mScaleKeys[i].Key.z,
				animation.mScaleKeys[i].time);
		}
	
}

void WallG::Graphics::AnimationIO::ReadAnimation(FILE* file, Animation& animation)
{
	uint32_t positionSize = 0;
	fscanf_s(file, "PositionSize: %d\n", &positionSize);
	animation.mPositionKeys.resize(positionSize);
	for (uint32_t i = 0; i < animation.mPositionKeys.size(); ++i)
	{
		fscanf_s(file, "%f %f %f %f\n",
			&animation.mPositionKeys[i].Key.x,
			&animation.mPositionKeys[i].Key.y,
			&animation.mPositionKeys[i].Key.z,
			&animation.mPositionKeys[i].time);
	}

	uint32_t rotationSize = 0;
	fscanf_s(file, "RotationSize: %d\n", &rotationSize);
	animation.mRotationKeys.resize(rotationSize);
	for (uint32_t i = 0; i < animation.mRotationKeys.size(); ++i)
	{
		fscanf_s(file, "%f %f %f %f %f\n",
			&animation.mRotationKeys[i].Key.w,
			&animation.mRotationKeys[i].Key.x,
			&animation.mRotationKeys[i].Key.y,
			&animation.mRotationKeys[i].Key.z,
			&animation.mRotationKeys[i].time);
	}

	uint32_t ScaleSize = 0;
	fscanf_s(file, "ScaleSize: %d\n", &ScaleSize);
	animation.mScaleKeys.resize(ScaleSize);
	for (uint32_t i = 0; i < animation.mScaleKeys.size(); ++i)
	{
		fscanf_s(file, "%f %f %f %f\n",
			&animation.mScaleKeys[i].Key.x,
			&animation.mScaleKeys[i].Key.y,
			&animation.mScaleKeys[i].Key.z,
			&animation.mScaleKeys[i].time);
	}
}