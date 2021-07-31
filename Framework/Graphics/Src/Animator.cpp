#include "Precompiled.h"
#include "Animator.h"
#include "AnimationUtil.h"
#include "ConstantBuffer.h"

using namespace WallG::Graphics;
using namespace WallG::Math;

 void WallG::Graphics::Animator::Initialize(const Model* model)
{
	mModel = model;
}

void WallG::Graphics::Animator::Terminate()
{
	mModel = nullptr;
	mAnimationTimer = 0.0f;
}

void WallG::Graphics::Animator::Update(float deltaTime)
{
	if (mAnimate)
	{
		auto& animationClip = mModel->animSet[mAnimaIndex];
		mAnimationTimer += animationClip->ticksPersecond * deltaTime;
		if (mLoop && mAnimationTimer > animationClip->duration)
			mAnimationTimer -= animationClip->duration;
		GetAnimationTransForm(*(mModel->skeleton), *mModel->animSet[mAnimaIndex], mAnimationTimer, mSkeletonTransforms);
	}
	else
		GetRootTransform(*(mModel->skeleton), mSkeletonTransforms);

	mToLocalTransforms.resize(mSkeletonTransforms.size());
	for (auto& bone : mModel->skeleton->bones)
	{
		mToLocalTransforms[bone->index] = bone->offsetTransform * mSkeletonTransforms[bone->index];
	}
}

void WallG::Graphics::Animator::Play()
{
	mAnimate = true;
}

void WallG::Graphics::Animator::Stop()
{
	mAnimate = false;
}


const std::vector<WallG::Math::Matrix4>& WallG::Graphics::Animator::GetSkeletonTransforms() const
{
    return mSkeletonTransforms;
}

const std::vector<WallG::Math::Matrix4>& WallG::Graphics::Animator::GetToLocalTransforms() const
{
    return mToLocalTransforms;
}
