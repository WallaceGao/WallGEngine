#include "ShipService.h"
#include "ShipComponent.h"
#include "UniverseService.h"

using namespace WallG;
using namespace WallG::Graphics;
using namespace WallG::Input;
using namespace WallG::Math;

// GameWorld
//   +
//   |
//   +---- GameObject
//   |       +
//   |       |
//   |       +---- Component
//   |
//   +---- Service

void ShipService::Initialize()
{
    mUniverseService = GetWorld().GetService<UniverseService>();
}

void ShipService::Update(float deltaTime)
{
    auto inputSystem = InputSystem::Get();

    if (inputSystem->IsKeyPressed(KeyCode::SPACE))
    {
        char shipName[128];
        //%03d is keep the number as 000 
        sprintf_s(shipName, "Ship%03d", ++mNextId);

        const GameObject* earth = mUniverseService->GetPlanet("Perseus");
        
        WallG::GameObject* newObject;
       
        int ranShip = rand()%2 + 0;
        Math::RandomFloat();
        switch (ranShip)
        {
        case 0:
            newObject =  GetWorld().CreatGameObject("../../Assets/DynamicUniverse/SmallShip.json", shipName);
            newObject->GetComponent<TransformComponent>()->SetPosition(earth->GetComponent<TransformComponent>()->GetPosition());
            break;
        case 1:
            newObject = GetWorld().CreatGameObject("../../Assets/DynamicUniverse/LagerShip.json", shipName);
            newObject->GetComponent<TransformComponent>()->SetPosition(earth->GetComponent<TransformComponent>()->GetPosition());
            break;
        default:
            break;
        }
    }
}

void ShipService::Register(const ShipComponent* shipComponent)
{
    const GameObject& planetObject = shipComponent->GetOwner();
    Entry& entry = mShipEntries.emplace_back();
    entry.shipComponent = shipComponent;
}

void ShipService::Unregister(const ShipComponent* shipComponent)
{
    auto match = [&](const auto& entry) {return (entry.shipComponent == shipComponent); };
    auto iter = std::find_if(mShipEntries.begin(), mShipEntries.end(), match);
    if (iter != mShipEntries.end())
    {
        mShipEntries.erase(iter);
    }
}
