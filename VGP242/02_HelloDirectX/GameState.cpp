#include "GameState.h"

using namespace WallG::Graphics;
using namespace WallG::Input;

void GameState::Initialize()
{
	auto graphicsSystem = GraphicsSystem::Get();
	graphicsSystem->SetClearColor(Colors::HotPink);
}

void GameState::Update(float deltaTime)
{

	auto inputSystem = InputSystem::Get();
	auto graphicsSystem = GraphicsSystem::Get();

	if (inputSystem->IsKeyPressed(WallG::Input::KeyCode::ONE))
	{
		graphicsSystem->SetClearColor(WallG::Graphics::Colors::LightGreen);
	}
	
	if (inputSystem->IsKeyPressed(WallG::Input::KeyCode::TWO))
	{
		graphicsSystem->SetClearColor(WallG::Graphics::Colors::Orange);
	}
	
	if (inputSystem->IsKeyPressed(WallG::Input::KeyCode::THREE))
	{
		graphicsSystem->SetClearColor(WallG::Graphics::Colors::Azure);
	}
}
