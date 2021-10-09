#pragma once

#include "GameObjectHandle.h"
#include "Service.h"

namespace WallG
{
	class CameraService final : public Service
	{
	public:
		SET_TYPE_ID(ServiceId::Camera)
		
		void Initialize() override;
		void Update(float deltaTime) override;
		void Render() override;
		void DebugUI() override;

		void SetCameraFocus(GameObjectHandle targetHandle, float distance);

		Camera& GetCamera() { return mCamera; };
		const Camera& GetCamera() const { return mCamera; };
		Camera& GetLightCamera() { return mLightCamera; };
		const Camera& GetLightCamera() const { return mLightCamera; };
		Camera& GetActiveCamera() { return *mActiveCamera; };
		const Camera& GetActiveCamera() const { return *mActiveCamera; };
	
		void SetCameraMoveSpeed(const float speed) { mCameraMoveSpeed = speed; }
		void SetCameraMoveSpeedFast(const float speed) { mCameraMoveSpeedFast = speed; }
	private:

		WallG::Graphics::Camera mCamera;
		WallG::Graphics::Camera mLightCamera;
		WallG::Graphics::Camera* mActiveCamera = nullptr;

		GameObjectHandle mTargetHandle;
		WallG::Math::Vector3 mOffset;

		float mLightCameraDistance = 100.0f;
		float mCameraMoveSpeed = 10.0f;
		float mCameraMoveSpeedFast = 100.0f;
		float mTargetDistance = 0.0f;
		float mFocusSpeed = 0.15f; // between 0 to 1
		float mNearPlane = 1.0f;
		float mFarPlane = 200.0f;
		float mAspectRatio = 1.0f;
		bool mLockedOn = false;
	};
}