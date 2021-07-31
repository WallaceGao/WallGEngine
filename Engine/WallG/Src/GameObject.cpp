#include "Precompiled.h"
#include "GameObject.h"

#include "Component.h"

using namespace WallG;

MEMPOOL_DEFINE(GameObject, 1000);

void GameObject::Initialize()
{
	ASSERT(!mInitialized, "GameObject - Already initialized");
	for (auto& Component : mComponets)
		Component->Initialize();
	mInitialized = true;	
}

void GameObject::Terminate()
{
	for (auto& Component : mComponets)
		Component->Terminate();
}

void GameObject::Update(float deltaTime)
{
	for (auto& Component : mComponets)
		Component->Update(deltaTime);
}


void GameObject::DebugUI()
{
	for (auto& Component : mComponets)
		Component->DebugUI();
}

void WallG::GameObject::AddComponent(Component* newComponent)
{
	mComponets.emplace_back(newComponent);
}


