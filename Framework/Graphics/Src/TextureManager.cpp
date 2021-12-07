#include "Precompiled.h"
#include "TextureManager.h"

#include "Texture.h"

using namespace WallG;
using namespace WallG::Graphics;

namespace
{
	std::unique_ptr<TextureManager> sTextureManager = nullptr;
}

void TextureManager::StaticInitialize(const char* root)
{
	ASSERT(sTextureManager == nullptr, "[TextureManager] Manager already initialized!");
	sTextureManager = std::make_unique<TextureManager>();
	sTextureManager->SetRootPath(root);
}

void TextureManager::StaticTerminate()
{
	if (sTextureManager != nullptr)
	{
		sTextureManager->Clear();
		sTextureManager.reset();
	}
}

TextureManager* TextureManager::Get()
{
	ASSERT(sTextureManager != nullptr, "[TextureManager] No instance registered.");
	return sTextureManager.get();
}

WallG::Graphics::TextureManager::TextureManager()
{
}

WallG::Graphics::TextureManager::~TextureManager()
{
	ASSERT(mInventory.empty(), "[TextureManager] Clear() must be called to clean up.");
}

void TextureManager::SetRootPath(const char* path)
{
	mRoot = path;
}

TextureId TextureManager::Load(const char* fileName)
{
	std::string fullName = mRoot + "/" + fileName;

	std::hash<std::string> hasher;
	TextureId hash = hasher(fullName);

	auto result = mInventory.insert({ hash, nullptr });
	if (result.second)
	{
		Texture* texture = new Texture;
		texture->Initialize(fullName.c_str());
		if (texture->GetIsInitialize())
		{
			result.first->second = texture;
		}
		else
		{
			delete texture;
			texture = nullptr;
			mInventory.erase(result.first);
			hash = 0;
		}
	}

	return hash;
}

void TextureManager::Clear()
{
	for (auto& item : mInventory)
	{
		if (item.second)
		{
			item.second->Terminate();
			delete item.second;
			item.second = nullptr;
		}
	}
	mInventory.clear();
}

void TextureManager::BindVS(TextureId id, uint32_t slot)
{
	auto iter = mInventory.find(id);
	if (iter != mInventory.end())
	{
		iter->second->BindVS(slot);
	}
}

void TextureManager::BindPS(TextureId id, uint32_t slot)
{
	auto iter = mInventory.find(id);
	if (iter != mInventory.end())
	{
		iter->second->BindPS(slot);
	}
}

TextureId TextureManager::LoadTexture(const char* fileName)
{
	return TextureManager::Get()->Load(fileName);
}

Texture* TextureManager::GetTexture(TextureId id)
{
	auto iter = mInventory.find(id);
	return iter != mInventory.end() ? iter->second : nullptr;
}