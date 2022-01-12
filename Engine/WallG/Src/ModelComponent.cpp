#include "Precompiled.h"
#include "ModelComponent.h"

#include "GameObject.h"
#include "GameWorld.h"
#include "RenderService.h"


using namespace WallG;
MEMPOOL_DEFINE(ModelComponent, 1000);

void ModelComponent::Initialize()
{
	auto renderService = GetOwner().GetWorld().GetService<RenderService>();
	renderService->Register(this);

	mModelId = ModelManager::Get()->LoadModel(mFileName);
}

void ModelComponent::Terminate()
{
	mModelId = 0;

	auto renderService = GetOwner().GetWorld().GetService<RenderService>();
	renderService->Unregister(this);
}

const Graphics::Model* ModelComponent::GetModel() const
{
	return ModelManager::Get()->GetModel(mModelId);
}




