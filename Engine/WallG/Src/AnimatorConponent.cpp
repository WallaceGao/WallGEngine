#include "Precompiled.h"
#include "AnimatorComponent.h"

#include "GameObject.h"
#include "ModelComponent.h"

using namespace WallG;

MEMPOOL_DEFINE(AnimatorComponent, 1000);

void AnimatorComponent::Initialize()
{
	mModelComponent = GetOwner().GetComponent<ModelComponent>();
	mAnimator.Initialize(mModelComponent->GetModel());
	mAnimator.SetLoop(mLoop);
}

void AnimatorComponent::Terminate()
{
	mModelComponent = nullptr;
	mAnimator.Terminate();
}

void AnimatorComponent::Update(float deltaTime)
{
     mAnimator.Update(deltaTime);
}	

