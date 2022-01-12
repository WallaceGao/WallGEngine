#include "GameState.h"

using namespace WallG;
using namespace WallG::Graphics;
using namespace WallG::Input;
using namespace WallG::Math;


void GameState::Initialize()
{
    mParticleSetting01.mPosition = { -0.25f,0.0f,-1.0f };
    mParticleSetting02.mPosition = { 0.25f,0.0f,-1.0f };

    //{-0.7f, 0.0f, -3.0f}
    //{0.7f, 0.0f, -3.0f}

    //mGameWorld.AddService<SkyBoxService>();
    auto cameraService = mGameWorld.AddService<CameraService>();
    auto renderService = mGameWorld.AddService<RenderService>();

    mGameWorld.Initialize(1000);
    cameraService->GetCamera().SetPosition({0.0f,0.0f,-10.0f});
    renderService->SetShadow(true);

    //mGameObjectHandle = mGameWorld.CreatGameObject("../../Assets/DynamicUniverse/SmallShip.json", "SmallShip")->GetHandle();
    mGameObjectHandle = mGameWorld.CreatGameObject("../../Assets/DynamicUniverse/LagerShip.json", "LagerShip")->GetHandle();
    //mGameObjectHandle = mGameWorld.CreatGameObject("../../Assets/Characters/Keli.json", "Kili")->GetHandle();
    //mAnimator.Initialize(&mGreatSwordStrafe);
    mParticleEmitter01.Initialize();
    mParticleEmitter01.SetPosition(mParticleSetting01.mPosition);
    mParticleEmitter01.SetStartSize(mParticleSetting01.mStartSize);
    mParticleEmitter01.SetEndSize(mParticleSetting01.mEndSize);
    mParticleEmitter01.SetMaxLifeTime(mParticleSetting01.mMaxLifeTime);
    mParticleEmitter01.SetStartColor(mParticleSetting01.mStartColor);
    mParticleEmitter01.SetEndColor(mParticleSetting01.mEndColor);

    mParticleEmitter02.Initialize();
    mParticleEmitter02.SetPosition(mParticleSetting02.mPosition);
    mParticleEmitter02.SetStartSize(mParticleSetting02.mStartSize);
    mParticleEmitter02.SetEndSize(mParticleSetting02.mEndSize);
    mParticleEmitter02.SetMaxLifeTime(mParticleSetting02.mMaxLifeTime);
    mParticleEmitter02.SetStartColor(mParticleSetting02.mStartColor);
    mParticleEmitter02.SetEndColor(mParticleSetting02.mEndColor);
}

void GameState::Terminate()
{
    mGameWorld.Terminate();
    mParticleEmitter01.Terminate();
    mParticleEmitter02.Terminate();
}

void GameState::Update(float deltaTime)
{
    mFPS = 1.0f / deltaTime;
    auto inputSystem = InputSystem::Get();

    if (inputSystem->IsKeyPressed(KeyCode::SPACE))
    {
        mParticleEmitter01.Start(180.0f);
        mParticleEmitter02.Start(180.0f);
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

    mParticleEmitter01.Update(deltaTime);
    mParticleEmitter02.Update(deltaTime);
    mGameWorld.Update(deltaTime);
}

void GameState::Render()
{
    mGameWorld.Render();
    mParticleEmitter01.Render(mGameWorld.GetService<CameraService>()->GetCamera());
    mParticleEmitter02.Render(mGameWorld.GetService<CameraService>()->GetCamera());

    mParticleEmitter01.SetPosition(mParticleSetting01.mPosition);
    mParticleEmitter01.SetStartSize(mParticleSetting01.mStartSize);
    mParticleEmitter01.SetEndSize(mParticleSetting01.mEndSize);
    mParticleEmitter01.SetMaxLifeTime(mParticleSetting01.mMaxLifeTime);
    mParticleEmitter02.SetPosition(mParticleSetting02.mPosition);
    mParticleEmitter02.SetStartSize(mParticleSetting02.mStartSize);
    mParticleEmitter02.SetEndSize(mParticleSetting02.mEndSize);
    mParticleEmitter02.SetMaxLifeTime(mParticleSetting02.mMaxLifeTime);
}

void GameState::DebugUI()
{
    mGameWorld.DebugUI();
    ImGui::Begin("Particle Test");
    if (ImGui::CollapsingHeader("particle01", ImGuiTreeNodeFlags_DefaultOpen))
    {
        ImGui::DragFloat3("Position01", (float*)&mParticleSetting01.mPosition, 0.1f);
        ImGui::DragFloat("StartSize01", &mParticleSetting01.mStartSize, 0.1f);
        ImGui::DragFloat("EndSize01", &mParticleSetting01.mEndSize, 0.1f);
        ImGui::DragFloat("MaxLifeTime01", &mParticleSetting01.mMaxLifeTime, 0.1f);
    }
    if (ImGui::CollapsingHeader("particle02", ImGuiTreeNodeFlags_DefaultOpen))
    {
        ImGui::DragFloat3("Position02", (float*)&mParticleSetting02.mPosition, 0.1f);
        ImGui::DragFloat("StartSize02", &mParticleSetting02.mStartSize, 0.1f);
        ImGui::DragFloat("EndSize02", &mParticleSetting02.mEndSize, 0.1f);
        ImGui::DragFloat("MaxLifeTime02", &mParticleSetting02.mMaxLifeTime, 0.1f);
    }
    ImGui::End();
}
