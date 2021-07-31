#pragma once
#include "Model.h"

namespace WallG::Graphics
{

	class Animator
	{
	public:
		void Initialize(const Model* model);
		void Terminate();
		void Update(float deltaTime);

		void Play();
		void Stop();

		const std::vector<WallG::Math::Matrix4>& GetSkeletonTransforms() const;
		const std::vector<WallG::Math::Matrix4>& GetToLocalTransforms() const;
		
		void SetLoop(bool isLoop) { mLoop = isLoop; }

	private:

		const Model* mModel = nullptr;

		std::vector<WallG::Math::Matrix4> mSkeletonTransforms;
		std::vector<WallG::Math::Matrix4> mToLocalTransforms;
		float mAnimationTimer = 0.0f;
		int mAnimaIndex = 0;

		bool mAnimate = false;
		bool mLoop = false;
	};
}
