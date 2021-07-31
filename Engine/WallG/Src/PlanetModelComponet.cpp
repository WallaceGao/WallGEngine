#include "Precompiled.h"
#include "PlanetModelComponet.h"

using namespace WallG;

MEMPOOL_DEFINE(PlanetComponent, 1000);

void WallG::PlanetComponent::Initialize()
{
	ModelComponent::Initialize();

	mModelId = ModelManager::Get()->NewEmptyModel(mTextureFilePath);
	Graphics::Model* model = const_cast<Graphics::Model*>(ModelManager::Get()->GetModel(mModelId));
	if (model->meshData.empty())
	{
		// If model is empty, manually make one
		auto& meshData = model->meshData.emplace_back(std::make_unique<Model::MeshData>());
		// 1.0f radius from json file
		meshData->meshBuffer.Initialize(MeshBuilder::CreatSphere(16,16,1.0f));
		// Add diffuse map here
		meshData->materiaIndex = 0;

		auto& materialData = model->materialData.emplace_back();
		materialData.diffuseMapName = mTextureFilePath;
		materialData.diffuseMap = std::make_unique<Graphics::Texture>();
		materialData.diffuseMap->Initialize(mTextureFilePath);
	}
}

void WallG::PlanetComponent::Terminate()
{
	ModelComponent::Terminate();
	mSpeed = 0.0f;
	mSelfSpeed = 0.0f;
}

void WallG::PlanetComponent::Update(float deltaTime)
{
	
}

void WallG::PlanetComponent::DebugUI()
{
	
}
