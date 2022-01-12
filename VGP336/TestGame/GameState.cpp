#include "GameState.h"

using namespace WallG;
using namespace WallG::Graphics;
using namespace WallG::Input;
using namespace WallG::Math;


void GameState::Initialize()
{

    mGameWorld.AddService<SkyBoxService>();
    auto cameraService = mGameWorld.AddService<CameraService>();
    auto renderService = mGameWorld.AddService<RenderService>();

    mGameWorld.Initialize(1000);
    cameraService->GetCamera().SetPosition({0.0f,0.0f,-10.0f});
    renderService->SetShadow(true);


    //Jannedaarc
    mGameObjectHandle = mGameWorld.CreatGameObject("../../Assets/Environment/Stage.json", "Stage")->GetHandle();
    mGameObjectHandle = mGameWorld.CreatGameObject("../../Assets/Characters/YYZ.json", "Youyouzi")->GetHandle();
    //mGameObjectHandle = mGameWorld.CreatGameObject("../../Assets/Characters/Keli.json", "Kili")->GetHandle();
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
    mGameWorld.DebugUI();
}
