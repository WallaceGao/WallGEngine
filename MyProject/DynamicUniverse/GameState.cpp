#include "GameState.h"

using namespace WallG;
using namespace WallG::Graphics;
using namespace WallG::Input;
using namespace WallG::Math;

namespace
{
    bool ReadOverride(const char* componentName, const rapidjson::JsonMember& member, GameObject& gameObject)
    {
		if (strcmp(componentName, "PlanetComponent") == 0)
		{
			auto planet = gameObject.AddComponent<PlanetComponent>();
			if(member.value.HasMember("Path"))
			{
				const auto& path = member.value["Path"].GetString();
				planet->SetTextureFilePath(path);
			}
			if (member.value.HasMember("Speed"))
			{
				const auto& speed = member.value["Speed"].GetFloat();
				planet->SetSpeed(speed);
			}
			if (member.value.HasMember("SelfSpeed"))
			{
				const auto& selfSpeed = member.value["SelfSpeed"].GetFloat();
				planet->SetSelfSpeed(selfSpeed);
			}
			return true;
		}

		return false;
	}
}

void GameState::Initialize()
{
	//mGameWorld.AddService<UniverseService>();
	mGameWorld.AddService<RenderService>();
	mGameWorld.Initialize(1000);

	GameObjectIO::SetReadOverride(ReadOverride);
	mGameObjectHandle = mGameWorld.CreatGameObject("../../Assets/DynamicUniverse/Sun.json", "Sun") -> GetHandle();
	mGameObjectHandle = mGameWorld.CreatGameObject("../../Assets/DynamicUniverse/Earth.json", "Earth") -> GetHandle();
	mGameObjectHandle = mGameWorld.CreatGameObject("../../Assets/DynamicUniverse/Jupiter.json", "Jupiter") -> GetHandle();
	mGameObjectHandle = mGameWorld.CreatGameObject("../../Assets/DynamicUniverse/Mercury.json", "Mercury") -> GetHandle();
	mGameObjectHandle = mGameWorld.CreatGameObject("../../Assets/DynamicUniverse/Moon.json", "Moon") -> GetHandle();
	mGameObjectHandle = mGameWorld.CreatGameObject("../../Assets/DynamicUniverse/Venus.json", "Venus") -> GetHandle();
}

void GameState::Terminate()
{
    mGameWorld.Terminate();
}

void GameState::Update(float deltaTime)
{
    mFPS = 1.0f / deltaTime;
    mGameWorld.Update( deltaTime);
}

void GameState::Render()
{
    mGameWorld.Render();
}

void GameState::DebugUI()
{
    ImGui::Begin("Debug Info", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
    ImGui::Text("fps: %f", mFPS);

    ImGui::End();
}
