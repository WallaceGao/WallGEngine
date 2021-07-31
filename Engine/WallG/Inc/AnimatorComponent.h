#pragma once

#include "Component.h"

namespace WallG
{
	class ModelComponent;

	class AnimatorComponent final : public Component
	{
	public:
		SET_TYPE_ID(ComponentId::Animator);
		MEMPOOL_DECLARE;

		AnimatorComponent() = default;

		void Initialize() override;
		void Terminate() override;
		void Update(float deltaTime) override;

		void SetLoop(bool loop) { mLoop = loop; }
		Graphics::Animator& GetAnimator() { return mAnimator; }
		const Graphics::Animator& GetAnimator() const { return mAnimator; }
	private:
		const ModelComponent* mModelComponent = nullptr;
		Graphics::Model* mModel;
		Graphics::Animator mAnimator;
		bool mLoop;
	};
}