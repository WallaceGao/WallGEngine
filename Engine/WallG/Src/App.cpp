#include "Precompiled.h"
#include "App.h"

#include "AppState.h"

using namespace WallG;
using namespace WallG::Core;
using namespace WallG::Graphics;
using namespace WallG::Input;
using namespace WallG::Math;

void WallG::App::ChangeState(const std::string& name)
{
	auto iter = mAppState.find(name);
	if (iter != mAppState.end())
	{
		LOG("App -- Changing state: %s", name.c_str());
		mCurrentState = iter->second.get();
	}
}

void WallG::App::Run(const AppConfig& appConfig)
{
	LOG("App -- Running ...");

	LOG("App -- Creating window");

	Window window;
	// char - 1 byte - 8 bits - can store 0000 0000 or 1111 1111 or 255,
	// wchar - 2 byte - 16 bits - unicode characters
	window.Initialize(GetModuleHandle(NULL), appConfig.appName, appConfig.windowWidth, appConfig.windowHeight);

	auto handle = window.GetWindowHandle();

	InputSystem::StaticInitialize(handle);
	GraphicsSystem::StaticInitialize(handle, false);
	ModelManager::StaticInitialize();
	SimpleDraw::StaticInitialize();
	DebugUI::StaticInitialize(handle, false, false);

	mCurrentState->Initialize();

	mRunning = true;
	while (mRunning && window.IsActive())
	{

		window.ProcessMessage();
		if (!window.IsActive())
		{
			continue;
		}
		if (mNextState)
		{
			LOG("APP--Changing app state");
			mCurrentState->Terminate();
			mCurrentState = std::exchange(mNextState, nullptr);
			mCurrentState->Initialize();
		}

		auto inputSystem = InputSystem::Get();
		inputSystem->Update();

		if (appConfig.escToQuit && inputSystem->IsKeyPressed(WallG::Input::KeyCode::ESCAPE))
		{
			LOG("App -- Esc pressed. Exiting App");
			Quit();
			continue;
		}

		auto deltaTime = TimeUtil::GetDeltaTime();
		mCurrentState->Update(deltaTime);

		auto graphicsSystem = GraphicsSystem::Get();
		graphicsSystem->BeginRender();

		//Draw staff here
		mCurrentState->Render();
		DebugUI::BeginRender();
		mCurrentState->DebugUI();
		DebugUI::EndRender();

		graphicsSystem->EndRender();
	}

	GraphicsSystem::StaticTerminate();
	InputSystem::StaticTerminate();

	//clearn up
	mCurrentState->Terminate();
	
	DebugUI::StaticTerminate();
	SimpleDraw::StaticTerminate();
	ModelManager::StaticTerminate();
	GraphicsSystem::StaticTerminate();
	InputSystem::StaticTerminate();

	window.Terminate();
}

