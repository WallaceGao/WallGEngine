#include "Precompiled.h"
#include "CameraService.h"

#include "GameObject.h"
#include "GameWorld.h"
#include "TransformComponent.h"

using namespace WallG;
using namespace WallG::Graphics;
using namespace WallG::Input;
using namespace WallG::Math;

void CameraService::Initialize()
{
	mCamera.SetPosition({ 0.0f, 0.0f, -300.0f });
}

void CameraService::Update(float deltaTime)
{
    auto inputSystem = InputSystem::Get();

    const float moveSpeed = inputSystem->IsKeyDown(KeyCode::LSHIFT) ? 10000.0f : 1000.0f;
    const float turnSpeed = 10.0f * Constants::DegToRad;

    const GameObject* target = GetWorld().GetGameObject(mTargetHandle);
    if (target != nullptr)
    {
        // Stop focus when any key is pressed
        if (inputSystem->IsKeyPressed(KeyCode::W) ||
            inputSystem->IsKeyPressed(KeyCode::S) ||
            inputSystem->IsKeyPressed(KeyCode::D) ||
            inputSystem->IsKeyPressed(KeyCode::A))
            mTargetHandle = GameObjectHandle();

        if (inputSystem->IsMouseDown(MouseButton::RBUTTON))
        {
            mCamera.Yaw(inputSystem->GetMouseMoveX() * turnSpeed * deltaTime);
            mCamera.Pitch(inputSystem->GetMouseMoveY() * turnSpeed * deltaTime);
        }
    }
    else
    {
        if (inputSystem->IsKeyDown(KeyCode::W))
            mCamera.Walk(moveSpeed * deltaTime);
        if (inputSystem->IsKeyDown(KeyCode::S))
            mCamera.Walk(-moveSpeed * deltaTime);
        if (inputSystem->IsKeyDown(KeyCode::D))
            mCamera.Strafe(moveSpeed * deltaTime);
        if (inputSystem->IsKeyDown(KeyCode::A))
            mCamera.Strafe(-moveSpeed * deltaTime);
        if (inputSystem->IsMouseDown(MouseButton::RBUTTON))
        {
            mCamera.Yaw(inputSystem->GetMouseMoveX() * turnSpeed * deltaTime);
            mCamera.Pitch(inputSystem->GetMouseMoveY() * turnSpeed * deltaTime);
        }
    }
}

void CameraService::Render()
{
    const GameObject* target = GetWorld().GetGameObject(mTargetHandle);
    if (target != nullptr)
    {
        auto targetTransform = target->GetComponent<TransformComponent>();
        auto targetPosition = targetTransform->GetPosition() - (mCamera.GetDirection() * mTargetDistance);
        auto cameraPosition = Math::Lerp(mCamera.GetPosition(), targetPosition, mFocusSpeed);
        mCamera.SetPosition(cameraPosition);
    }
}

void CameraService::DebugUI()
{

}

void CameraService::SetCameraFocus(GameObjectHandle targetHandle, float distance)
{
    mTargetHandle = targetHandle;
    mTargetDistance = distance;
}