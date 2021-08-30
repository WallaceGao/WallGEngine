#include "GameState.h"

#include "DynamicUniverseTypes.h"
#include "FactoryComponent.h"
#include "FactoryService.h"
#include "PlanetComponent.h"
#include "ShipComponent.h"
#include "ShipService.h"
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
			if (member.value.HasMember("DistanceFromParent"))
			{
				const auto& distance = member.value["DistanceFromParent"].GetFloat();
				planet->SetDistanceFromParent(distance);
			}
			if (member.value.HasMember("ResourceType"))
			{
				const auto& resourceType = member.value["ResourceType"].GetInt();
				planet->SetResourceType(resourceType);
			}
			return true;
		}
		else if (strcmp(componentName, "FactoryComponent") == 0)
		{
			auto factory = gameObject.AddComponent<FactoryComponent>();
			if (member.value.HasMember("MaxIronCapacity"))
			{
				const auto& maxIronCapacity = member.value["MaxIronCapacity"].GetFloat();
				factory->SetMaxIronCapacity(maxIronCapacity);
			}
			if (member.value.HasMember("MaxCopperCapacity"))
			{
				const auto& maxCopperCapacity = member.value["MaxCopperCapacity"].GetFloat();
				factory->SetMaxCopperCapacity(maxCopperCapacity);
			}
			if (member.value.HasMember("EngineBuildTime"))
			{
				const auto& EngineBuildTime = member.value["EngineBuildTime"].GetFloat();
				factory->SetEngineBuildTime(EngineBuildTime);
			}
			if (member.value.HasMember("CargoBuildTime"))
			{
				const auto& cargoBuildTime = member.value["CargoBuildTime"].GetFloat();
				factory->SetCargoBuildTime(cargoBuildTime);
			}
			if (member.value.HasMember("MineToolBuildTime"))
			{
				const auto& mineToolBuildTime = member.value["MineToolBuildTime"].GetFloat();
				factory->SetMineToolBuildTime(mineToolBuildTime);
			}
			if (member.value.HasMember("IronPrice"))
			{
				const auto& ironPrice = member.value["IronPrice"].GetFloat();
				factory->SetIronPrice(ironPrice);
			}
			if (member.value.HasMember("MinIronPrice"))
			{
				const auto& minIronPrice = member.value["MinIronPrice"].GetFloat();
				factory->SetMinIronPrice(minIronPrice);
			}
			if (member.value.HasMember("MaxIronPrice"))
			{
				const auto& maxIronPrice = member.value["MaxIronPrice"].GetFloat();
				factory->SetMaxIronPrice(maxIronPrice);
			}
			if (member.value.HasMember("CopperPrice"))
			{
				const auto& copperPrice = member.value["CopperPrice"].GetFloat();
				factory->SetCopperPrice(copperPrice);
			}
			if (member.value.HasMember("MinCopperPrice"))
			{
				const auto& minCopperPrice = member.value["MinCopperPrice"].GetFloat();
				factory->SetMinCopperPrice(minCopperPrice);
			}
			if (member.value.HasMember("MaxCopperPrice"))
			{
				const auto& maxCopperPrice = member.value["MaxCopperPrice"].GetFloat();
				factory->SetMaxCopperPrice(maxCopperPrice);
			}
		}
		else if (strcmp(componentName, "ShipComponent") == 0)
		{
			auto ship = gameObject.AddComponent<ShipComponent>();
			if (member.value.HasMember("Speed"))
			{
				const auto& speed = member.value["Speed"].GetFloat();
				ship->SetSpeed(speed);
			}
			if (member.value.HasMember("MineSpeed"))
			{
				const auto& mineSpeed = member.value["MineSpeed"].GetFloat();
				ship->SetSpeed(mineSpeed);
			}
			if (member.value.HasMember("CarGoSize"))
			{
				const auto& carGoSize = member.value["CarGoSize"].GetFloat();
				ship->SetCargoSize(carGoSize);
			}
		}
		return false;
	}
}

void GameState::Initialize()
{
	mGameWorld.AddService<UniverseService>();
	mGameWorld.AddService<RenderService>();
	mGameWorld.AddService<ShipService>();
	mGameWorld.AddService<FactoryService>();
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
