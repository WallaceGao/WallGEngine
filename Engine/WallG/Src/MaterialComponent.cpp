#include "Precompiled.h"
#include "MaterialComponent.h"

#include "GameObject.h"

using namespace WallG;
using namespace WallG::Graphics;
using namespace WallG::Input;
using namespace WallG::Math;

MEMPOOL_DEFINE(MaterialComponent, 1000);

void WallG::MaterialComponent::Initialize()
{
    mMaterial.ambient = { 0.35f };
    mMaterial.specular = { 0.5f };
    mMaterial.diffuse = { 0.5f };
    mMaterial.Power = { 5.0f };

    mMaterialBuffer.Initialize();
    
}

void WallG::MaterialComponent::Terminate()
{
    mMaterialBuffer.Terminate();
}
