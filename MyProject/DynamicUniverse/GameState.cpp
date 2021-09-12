#include "GameState.h"

#include "DynamicUniverseTypes.h"
#include "EconomicService.h"
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
			if (member.value.HasMember("Scale"))
			{
				const auto& planetScale = member.value["Scale"].GetArray();
				float x = planetScale[0].GetFloat();
				float y = planetScale[1].GetFloat();
				float z = planetScale[2].GetFloat();
				planet->SetPlanetScale({x,y,z});
			}
			if (member.value.HasMember("StartRota"))
			{
				const auto& startRota = member.value["StartRota"].GetFloat();
				planet->SetStartRota(startRota);
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
				ship->SetMineSpeed(mineSpeed);
			}
			if (member.value.HasMember("CarGoSize"))
			{
				const auto& carGoSize = member.value["CarGoSize"].GetFloat();
				ship->SetCargoSize(carGoSize);
			}
			if (member.value.HasMember("ShipType"))
			{
				const auto& shipType = member.value["ShipType"].GetInt();
				ship->SetShipType(shipType);
			}
			if (member.value.HasMember("Money"))
			{
				const auto& money = member.value["Money"].GetFloat();
				ship->SetMoney(money);
			}
		}
		return false;
	}
}

void GameState::Initialize()
{
	auto cameraService = mGameWorld.AddService<CameraService>();
	mGameWorld.AddService<UniverseService>();
	mGameWorld.AddService<SkyBoxService>();
	mGameWorld.AddService<RenderService>();
	mGameWorld.AddService<ShipService>();
	mGameWorld.AddService<EconomicService>();
	mGameWorld.AddService<FactoryService>();
	mGameWorld.Initialize(1000);
	
	// We will create planets first before moons, this way
	// we can guarantee that planets will update their transforms first
	// so the moon and transform around them.
	GameObjectIO::SetReadOverride(ReadOverride);
	mGameWorld.CreatGameObject("../../Assets/DynamicUniverse/Skybox.json", "Skybox");
	mGameWorld.CreatGameObject("../../Assets/DynamicUniverse/Sun.json", "Sun");
	mGameWorld.CreatGameObject("../../Assets/DynamicUniverse/Hot.json", "Lepus");
	mGameWorld.CreatGameObject("../../Assets/DynamicUniverse/Ice.json", "Hydrus");
	mGameWorld.CreatGameObject("../../Assets/DynamicUniverse/Lava.json", "Gemini");
	mGameWorld.CreatGameObject("../../Assets/DynamicUniverse/Metal.json", "Draco");
	// 
	auto Perseus = mGameWorld.CreatGameObject("../../Assets/DynamicUniverse/Earth2.json", "Perseus");
	auto Microscopium = mGameWorld.CreatGameObject("../../Assets/DynamicUniverse/Earth1.json", "Microscopium");
	// 
	auto Andromeda = mGameWorld.CreatGameObject("../../Assets/DynamicUniverse/Moon1.json", "Andromeda");
	auto Antlia = mGameWorld.CreatGameObject("../../Assets/DynamicUniverse/Moon2.json", "Antlia");
	auto Aquila = mGameWorld.CreatGameObject("../../Assets/DynamicUniverse/Moon3.json", "Aquila");
	
	// Set the moon to follow the earth's transform
	Andromeda->GetComponent<PlanetComponent>()->SetParentPlanet(Perseus->GetHandle());
	Antlia->GetComponent<PlanetComponent>()->SetParentPlanet(Perseus->GetHandle());
	Aquila->GetComponent<PlanetComponent>()->SetParentPlanet(Microscopium->GetHandle());

	mGameWorld.AddSelectedHandler([cameraService](GameObject* selectedObject)
	{
		float distance = 100.0f;
		if (auto planetComponent = selectedObject->GetComponent<PlanetComponent>(); planetComponent != nullptr)
		{
			distance = planetComponent->GetPlanetScale().x * 3.0f;
		}
		else if (auto shipComponent = selectedObject->GetComponent<ShipComponent>(); shipComponent != nullptr)
		{
			//distance = planetComponent->GetRadius() * 2.0f;
			distance = 30.0f;
		}

		cameraService->SetCameraFocus(selectedObject->GetHandle(), distance);
	});
}

void GameState::Terminate()
{
    mGameWorld.Terminate();
}

void GameState::Update(float deltaTime)
{
	static int fastForwardUpdates = 0;

	if (InputSystem::Get()->IsKeyPressed(KeyCode::ENTER))
	{
		fastForwardUpdates = 10000;
	}

	do
	{
		mGameWorld.Update(deltaTime);
		fastForwardUpdates--;
	} while (fastForwardUpdates > 0);
}

void GameState::Render()
{
    mGameWorld.Render();
}

void GameState::DebugUI()
{
	mGameWorld.DebugUI();
}
