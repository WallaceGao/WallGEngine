#include "GameState.h"

using namespace WallG;
using namespace WallG::AI;
using namespace WallG::Input;
using namespace WallG::Math;
using namespace WallG::Graphics;



void GameState::Initialize()
{
	auto cameraService = mGameWorld.AddService<CameraService>();
	mGameWorld.AddService<RenderService>();
	mGameWorld.Initialize(1000);
	cameraService->SetCameraTo3D(false);
}

void GameState::Terminate()
{
	mGameWorld.Terminate();
}

void GameState::Update(float deltaTime)
{
	Vector2 point1 = Vector2{ 0.0f,0.0f };
	Vector2 point2 = Vector2{ 250.0f,250.0f };
	Vector2 point3 = Vector2{ 750.0f,50.0f };
	SimpleDraw::AddLine2D(point1, point2, Colors::Orange);
	SimpleDraw::AddLine2D(point3, point1, Colors::Orange);
	SimpleDraw::AddLine2D(point3, point2, Colors::Orange);
}

void GameState::Render()
{
	auto cameraService =mGameWorld.GetService<CameraService>();
	SimpleDraw::Render(cameraService->GetCamera());
}

void GameState::DebugUI()
{

}


