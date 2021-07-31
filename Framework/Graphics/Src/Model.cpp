#include "Precompiled.h"
#include "Model.h"
#include "ModelLoader.h"

using namespace WallG;
using namespace WallG::Graphics;



void Model::Initialize(const std::filesystem::path& fileName)
{
	// This is the opposite of SaveModel in ModelImporter
	ModelLoader::LoadModel(fileName, *this);
	ModelLoader::LoadMaterial(fileName, *this);
	ModelLoader::LoadSkeleton(fileName, *this);
	ModelLoader::LoadAnimation(fileName, *this);

	// Initialize meshBuffers
	
	for (size_t i = 0; i < meshData.size(); ++i)
	{
		meshData[i]->meshBuffer.Initialize(meshData[i]->mesh);
	}

	std::filesystem::path fileDirectory = fileName.parent_path();

	// Load textures;
	for (size_t i = 0; i < materialData.size(); ++i)
	{
		auto& material = materialData[i];
		if (!material.diffuseMapName.empty())
		{
			std::filesystem::path texturePath = fileDirectory / material.diffuseMapName;
			material.diffuseMap = std::make_unique<Texture>();
			material.diffuseMap->Initialize(texturePath);
		}
		if (!material.normalMapName.empty())
		{
			std::filesystem::path texturePath = fileDirectory / material.normalMapName;
			material.normalMap = std::make_unique<Texture>();
			material.normalMap->Initialize(texturePath);
		}
		if (!material.specularMapName.empty())
		{
			std::filesystem::path texturePath = fileDirectory / material.specularMapName;
			material.specularMap = std::make_unique<Texture>();
			material.specularMap->Initialize(texturePath);
		}
	}
	// Load Skeleton 
	for (auto& bone : skeleton->bones)
	{
		
		if (bone->parentIndex != -1)
		{
			bone->parent = skeleton->bones[bone->parentIndex].get();
		}
		else
		{
			skeleton->root = bone.get();
		}

		for (size_t j = 0; j < bone->childIndices.size(); ++j)
		{
			bone->children.push_back((skeleton->bones[bone->childIndices[j]].get()));
		}
	}
}

void Model::Terminate()
{
	for (size_t i = 0; i < meshData.size(); ++i)
	{
		meshData[i]->meshBuffer.Terminate();
	}
	for (auto& material : materialData)
	{
		if (!material.diffuseMapName.empty())
		{
			material.diffuseMap->Terminate();
		}
		if (!material.normalMapName.empty())
		{
			material.normalMap->Terminate();
		}
		if (!material.specularMapName.empty())
		{
			material.specularMap->Terminate();
		}
	}
	meshData.clear();
	materialData.clear();
	skeleton.reset();
	animSet.clear();
}



void Model::Render() const
{
	for (size_t i = 0; i < meshData.size(); ++i)
	{
		const Model::MaterialData& material = materialData[meshData[i]->materiaIndex];
		if (material.diffuseMap)
			material.diffuseMap->BindPS(0);
		if (material.normalMap)
			material.normalMap->BindPS(3);
		meshData[i]->meshBuffer.Render();
	}
}

