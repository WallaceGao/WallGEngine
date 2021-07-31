#include "Precompiled.h"
#include "UniverseService.h"

#include "GameObject.h"
#include "PlanetModelComponet.h"
#include "TransformComponent.h"

using namespace WallG;
using namespace WallG::Graphics;
using namespace WallG::Input;
using namespace WallG::Math;

void UniverseService::Initialize()
{

    //mPlantPosition.resize(7);
    //mShipLocation.resize(3);
}

void UniverseService::Terminate()
{
    ////Ship
    //mShipMeshBuffer.Terminate();
    //mShipTexture.Terminate();
}

void UniverseService::Update(float deltaTime)
{


    //
    //auto direction = mPlantPosition[targetPlanet] - mShipLocation[0];
    //if (Magnitude(direction) < 10.0f)
    //{
    //    targetPlanet = std::rand() % 7;
    //}
    //mShipLocation[0] += Normalize(direction) * deltaTime * 50.0f;
}


void UniverseService::DebugUI()
{

}

//