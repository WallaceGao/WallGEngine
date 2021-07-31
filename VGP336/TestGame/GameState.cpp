#include "GameState.h"

using namespace WallG;
using namespace WallG::Graphics;
using namespace WallG::Input;
using namespace WallG::Math;


void GameState::Initialize()
{

    mGameWorld.AddService<RenderService>();
    mGameWorld.Initialize(1000);
    mGameObjectHandle = mGameWorld.CreatGameObject("../../Assets/Templates/Test.json", "Wallace")->GetHandle();
    //mAnimator.Initialize(&mGreatSwordStrafe);
}

void GameState::Terminate()
{
    mGameWorld.Terminate();
}

void GameState::Update(float deltaTime)
{
    mFPS = 1.0f / deltaTime;

    auto inputSystem = InputSystem::Get();

    if (inputSystem->IsKeyPressed(KeyCode::SPACE))
    {
        auto newObject = mGameWorld.CreatGameObject("../../Assets/Templates/Test.json", std::to_string(mNextId++));
        mNextPositionX += 1.0f;
        newObject->GetComponent<TransformComponent>()->SetPosition({mNextPositionX, 0.0f, 0.0f});

        if (mNextId >= 5)
        {
            auto kill = mGameWorld.FindGameObject(std::to_string(mNextId - 5));
            mGameWorld.Destroy(kill->GetHandle());
        }
    }

    if (inputSystem->IsKeyDown(KeyCode::LEFT))
    {
        mRotation.y += deltaTime;
    }
    if (inputSystem->IsKeyDown(KeyCode::RIGHT))
    {
        mRotation.y -= deltaTime;
    }

    if (inputSystem->IsKeyDown(KeyCode::UP))
    {
        mRotation.x += deltaTime;
    }
    if (inputSystem->IsKeyDown(KeyCode::DOWN))
    {
        mRotation.x -= deltaTime;
    }


    mGameWorld.Update(deltaTime);
}

void GameState::Render()
{
    mGameWorld.Render();
}

void GameState::DebugUI()
{
    ImGui::Begin("Debug Info", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
    ImGui::Text("fps: %f", mFPS);
    ImGui::DragFloat3("Rotation", (float*)&mRotation, 0.01f);


    ImGui::Checkbox("Show Skeleton", &mShowSkeleton);
	ImGui::Checkbox("Show Animation", &mShowAnimation);

    GameObject* GameObject = mGameWorld.GetGameObject(mGameObjectHandle);
    if (GameObject)
    {
        GameObject->DebugUI();
    }
    auto& animator = GameObject->GetComponent<AnimatorComponent>()->GetAnimator();
    if (mShowAnimation)
    {
        animator.Play();
    }
    else
    {
        animator.Stop();
    }

    ImGui::End();
}
