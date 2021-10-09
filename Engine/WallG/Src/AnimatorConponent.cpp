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

void AnimatorComponent::DebugUI()
{
	ImGui::Text(GetOwner().GetName().c_str());
	if (ImGui::CollapsingHeader("Show Skeleton Name", ImGuiTreeNodeFlags_DefaultOpen),false)
	{
		for (auto& bone : mModelComponent->GetModel()->skeleton->bones)
		{
			ImGui::Text(bone->name.c_str());
		}
	}
	ImGui::Checkbox("Show Skeleton", &mShowSkeleton);
	if (ImGui::Checkbox("Show Animation", &mShowAnimation))
	{
		if (mShowAnimation)
		{
			mAnimator.Play();
		}
		else
		{
			mAnimator.Stop();
		}
	}
}