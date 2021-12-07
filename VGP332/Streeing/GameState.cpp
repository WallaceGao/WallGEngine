#include "GameState.h"
#include "WallG/Inc/WallG.h"

#include "SCV.h"
#include "TypeIds.h"

using namespace WallG;
using namespace WallG::AI;
using namespace WallG::Input;
using namespace WallG::Graphics;

void GameState::Initialize()
{
	auto cameraService = mGameWorld.AddService<CameraService>();
	mGameWorld.AddService<RenderService>();
	mGameWorld.Initialize(1000);
	cameraService->SetCameraTo3D(false);

	auto& scv = scvs.emplace_back(std::make_unique<SCV>(world));
	scv->Initialize();
	scv->position.x = Math::RandomFloat(0.0f, (float)GraphicsSystem::Get()->GetBackBufferWidth());
	scv->position.y = Math::RandomFloat(0.0f, (float)GraphicsSystem::Get()->GetBackBufferHeight());

	world.AddObstacle(Math::Circle({ 100,300 }, 50.0f));
	world.AddObstacle(Math::Circle({ 250,400 }, 100.0f));
	world.AddObstacle(Math::Circle({ 300,100 }, 150.0f));
	world.AddObstacle(Math::Circle({ 800,800 }, 200.0f));
}

void GameState::Terminate()
{
	mGameWorld.Terminate();
}

void GameState::Update(float deltaTime)
{
	auto inputSystem = Input::InputSystem::Get();

	for (auto& scv : scvs)
	{
		scv->destination.x = (float)inputSystem->GetMouseScreenX();
		scv->destination.y = (float)inputSystem->GetMouseScreenY();
		scv->Update(deltaTime);
		
	}
}

void GameState::Render()
{
	auto cameraService = mGameWorld.GetService<CameraService>();
	SimpleDraw::Render(cameraService->GetCamera());

	for (auto& scv : scvs)
		scv->Render();
}

void GameState::DebugUI()
{
	for (auto& scv : scvs)
	{
		scv->ShowDebugUI();
	}
	world.DebugDraw();
}


