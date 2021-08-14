#include "GameState.h"

#include "DynamicUniverseTypes.h"
#include "PlanetComponent.h"
#include "ShipComponent.h"
#include "UniverseService.h"

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
			if (member.value.HasMember("Name"))
			{
				const auto& name = member.value["Name"].GetString();
				planet->SetName(name);
			}
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
			if (member.value.HasMember("Scale"))
			{
				const auto& scale = member.value["Scale"].GetFloat();
				planet->SetPlanetScale(scale);
			}
			if (member.value.HasMember("DistanceFromParent"))
			{
				const auto& distance = member.value["DistanceFromParent"].GetFloat();
				planet->SetDistanceFromParent(distance);
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
	
	// We will create planets first before moons, this way
	// we can guarantee that planets will update their transforms first
	// so the moon and transform around them.
	GameObjectIO::SetReadOverride(ReadOverride);
	mGameWorld.CreatGameObject("../../Assets/DynamicUniverse/Sun.json", "Sun");
	auto earth = mGameWorld.CreatGameObject("../../Assets/DynamicUniverse/Earth.json", "Earth");
	mGameWorld.CreatGameObject("../../Assets/DynamicUniverse/Jupiter.json", "Jupiter");
	mGameWorld.CreatGameObject("../../Assets/DynamicUniverse/Mercury.json", "Mercury");
	auto moon = mGameWorld.CreatGameObject("../../Assets/DynamicUniverse/Moon.json", "Moon");
	mGameWorld.CreatGameObject("../../Assets/DynamicUniverse/Venus.json", "Venus");

	// Set the moon to follow the earth's transform
	moon->GetComponent<PlanetComponent>()->SetParentPlanet(earth->GetHandle());
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

	mGameWorld.DebugUI();
}
