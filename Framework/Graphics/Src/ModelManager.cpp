#include "Precompiled.h"
#include "ModelManager.h"

using namespace WallG::Graphics;
using namespace WallG;

namespace
{
	std::unique_ptr<ModelManager> sModelManager;
}

void ModelManager::StaticInitialize()
{
	ASSERT(sModelManager == nullptr, "ModelManager -- Manager already initialized!");
	sModelManager = std::make_unique<ModelManager>();
}

void WallG::Graphics::ModelManager::StaticTerminate()
{
	if (sModelManager != nullptr)
	{
		sModelManager->Terminate();
		sModelManager.reset();
	}
}

ModelManager* WallG::Graphics::ModelManager::Get()
{
	ASSERT(sModelManager != nullptr, "ModelManager -- No manager registered.");
	return sModelManager.get();
}

void WallG::Graphics::ModelManager::Terminate()
{
	for (auto& entry : mModels)
		entry.model->Terminate();
	mModels.clear();
}

ModelId WallG::Graphics::ModelManager::LoadModel(std::filesystem::path filePath)
{
	// If we don't have a filename, just skip loading
	if (filePath.empty())
		return {};

	auto iter = std::find_if(mModels.begin(), mModels.end(), [&](const auto& entry) {
		return entry.name == filePath;
	});
	if (iter != mModels.end())
	{
		return std::distance(mModels.begin(), iter) + 1;
	}
	auto& entry = mModels.emplace_back();
	entry.name = filePath.u8string();
	entry.model = std::make_unique<Graphics::Model>();
	entry.model->Initialize(filePath);
	return mModels.size();
}

ModelId WallG::Graphics::ModelManager::NewEmptyModel(std::string name)
{
	// If we don't have a name, just skip
	if (name.empty())
		return {};

	auto iter = std::find_if(mModels.begin(), mModels.end(), [&](const auto& entry) {
		return entry.name == name;
	});
	if (iter != mModels.end())
	{
		return std::distance(mModels.begin(), iter) + 1;
	}
	auto& entry = mModels.emplace_back();
	entry.name = name;
	entry.model = std::make_unique<Graphics::Model>();
	return mModels.size();
}

const Model* WallG::Graphics::ModelManager::GetModel(ModelId id) const
{
	ASSERT(id > 0 && id <= mModels.size(), "ModelManager -- Invalid model id.");
	return mModels[id - 1].model.get();
}

