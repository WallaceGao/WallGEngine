#include "ShipService.h"

#include "PlanetComponent.h"
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

        const char* shipTemplates[] = {
            "../../Assets/DynamicUniverse/SmallShip.json",
            "../../Assets/DynamicUniverse/LagerShip.json"
        };

        const int index = Random::UniformInt(0, static_cast<int>(std::size(shipTemplates)) - 1);
        GameObject* newObject = GetWorld().CreatGameObject(shipTemplates[index], shipName);

        const PlanetComponent* homePlanet = mUniverseService->GetRandomHomePlanet();

        float planetSize = homePlanet->GetPlanetScale().x;
        float shipRotation = Random::RandomFloat(0, Constants::TwoPi);
        Vector3 planetPosition = homePlanet->GetOwner().GetComponent<TransformComponent>()->GetPosition();
        Vector3 shipDirection = TransformNormal(Vector3::ZAxis, Matrix4::RotationY(shipRotation));
        Vector3 shipPosition = planetPosition + (shipDirection * planetSize);
        newObject->GetComponent<TransformComponent>()->SetPosition(shipPosition);
        newObject->GetComponent<TransformComponent>()->SetRotation(Quaternion::RotationAxis(Vector3::YAxis, shipRotation));
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
