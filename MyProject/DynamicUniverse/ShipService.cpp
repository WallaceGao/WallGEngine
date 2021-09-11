#include "ShipService.h"
#include "ShipComponent.h"
#include "UniverseService.h"
#include <ImGui/Inc/ImPlot.h>

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
       
        int ramShip = rand()%2 + 0;
        Math::RandomFloat();
        switch (ramShip)
        {
        case 0:
            newObject =  GetWorld().CreatGameObject("../../Assets/DynamicUniverse/SmallShip.json", shipName);
            newObject->GetComponent<TransformComponent>()->SetPosition(earth->GetComponent<TransformComponent>()->GetPosition());
        case 1:
            newObject = GetWorld().CreatGameObject("../../Assets/DynamicUniverse/LagerShip.json", shipName);
            newObject->GetComponent<TransformComponent>()->SetPosition(earth->GetComponent<TransformComponent>()->GetPosition());
        default:
            break;
        }
    }

    static float waitTime = 0.0f;
    waitTime -= deltaTime;
    if (waitTime <= 0.0f)
    {
        waitTime += 0.5f;
        float time = Core::TimeUtil::GetTime();
        mPoints.push_back({ time, sin(time) });
    }
    if (mPoints.size() > 20)
        mPoints.erase(mPoints.begin());
}

void ShipService::DebugUI()
{
    ImGui::SetNextWindowSize({ 500.0f, 0.0f });
    ImGui::Begin("Test");

    if (ImPlot::BeginPlot("Copper", "x", "Y"))
    {
        std::vector<ImVec2> sin, cos;
        for (auto& pt : mPoints)
        {
            sin.push_back({ pt.x - mPoints[0].x, pt.y });
            cos.push_back({ pt.x - mPoints[0].x, -pt.y });
        }

        ImPlot::PlotLine("Sin", sin.data(), sin.size());
        ImPlot::PlotLine("Cos", cos.data(), cos.size());
        ImPlot::EndPlot();
    }

    if (ImPlot::BeginPlot("Iron", "x", "Y"))
    {
        std::vector<ImVec2> line;
        for (auto& pt : mPoints)
            line.push_back({ pt.x - mPoints[0].x, pt.y * 2.0f });

        ImPlot::PlotLine("Sin", line.data(), line.size());
        ImPlot::EndPlot();
    }

    ImGui::End();
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
