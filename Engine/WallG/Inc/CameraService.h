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
	
	private:
		WallG::Graphics::Camera mCamera;
		GameObjectHandle mTargetHandle;
		WallG::Math::Vector3 mOffset;
		float mTargetDistance = 0.0f;
		float mFocusSpeed = 0.15f; // between 0 to 1
		bool mLockedOn = false;
	};
}