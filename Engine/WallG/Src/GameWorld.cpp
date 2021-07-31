#include "Precompiled.h"
#include "GameWorld.h"

#include "GameObjectIO.h"
using namespace WallG;

void GameWorld::Initialize(uint32_t capacity)
{
	ASSERT(!mInitialized, "GameWorld - Already initialized");

	for (auto& service : mServices)
	{
		service->Initialize();
	}

	mGameObjects.resize(capacity);
	mFreeSlots.resize(capacity);
	std::iota(mFreeSlots.begin(), mFreeSlots.end(), 0);

	mInitialized = true;
}

void GameWorld::Terminate()
{
	if (!mInitialized)
		return;
	
	for (auto gameObject: mUpdateList)
		Destroy(gameObject->GetHandle());
	ProcessDestroyList();
	ASSERT(mUpdateList.empty(), "GameWorld - Failed to clean up game object.");

	for (auto& service : mServices)
		service->Terminate();

	mInitialized = false;
}

void GameWorld::Update(float deltaTime)
{
	ASSERT(!mUpdating, "GameWorld - Already updating the world");

	//This defers game object destriction
	mUpdating = true;

	for (auto& service: mServices)
	{
		service->Update(deltaTime);
	}
	for (size_t i = 0; i < mUpdateList.size();++i)
	{
		GameObject* gameObject = mUpdateList[i];
		if (IsValid(gameObject->GetHandle()))
		{
			gameObject->Update(deltaTime);
		}
	}

	//Allow game object destructions
	mUpdating = false;

	//Now we can safely destory objects
	ProcessDestroyList();
}

void GameWorld::Render()
{
	for (auto& service : mServices)
	{
		service->Render();
	}
}

void GameWorld::DebugUI()
{
	for (auto& service : mServices)
	{
		service->DebugUI();
	}
}

GameObject* WallG::GameWorld::CreatGameObject(const std::filesystem::path& filePath, std::string name)
{
	ASSERT(mInitialized, "GameWorld - world must be initialized first before creat game object");

	if (mFreeSlots.empty())
	{
		return nullptr;
	}

	FILE* file = nullptr;
	auto err = fopen_s(&file, filePath.u8string().c_str(), "r");
	if (err != 0 || file == nullptr)
	{
		LOG("Error:failed to open template file %s", filePath.u8string().c_str());
		return nullptr;
	}

	uint32_t freeSlot = mFreeSlots.back();
	mFreeSlots.pop_back();

	auto& slot = mGameObjects[freeSlot];
	auto& newObject = slot.gameobject;
	newObject = std::make_unique<GameObject>();

	GameObjectIO::Read(file, *newObject);
	fclose(file);

	// Initialize handle
	GameObjectHandle handle;
	handle.mID = freeSlot;
	handle.mGeneration = slot.generation;

	// Initialize game object
	newObject -> mWorld = this;
	newObject->mHandle = handle;
	newObject->mName = name;
	newObject->Initialize();

	// Add Game object to update list
	mUpdateList.push_back(newObject.get());

	return newObject.get();
}

GameObject* WallG::GameWorld::GetGameObject(GameObjectHandle handle)
{
	if (handle.mID >= 0 && handle.mID < mGameObjects.size())
	{
		auto& slot = mGameObjects[handle.mID];
		if (slot.generation == handle.mGeneration)
		{
			return slot.gameobject.get();
		}
	}
	return nullptr;
}

GameObject* GameWorld::FindGameObject(const std::string& name)
{
	GameObjectList::iterator iter = std::find_if(mUpdateList.begin(), mUpdateList.end(), [&](const GameObject* gameobecjt)
	{
		return gameobecjt->GetName() == name;
	});
	return iter == mUpdateList.end() ? nullptr : *iter;
}

void GameWorld::Destroy(GameObjectHandle handle)
{
	ASSERT(mInitialized, "GameWorld - World must be initialized first before destory game object");

	if (!IsValid(handle))
		return;

	auto& slot = mGameObjects[handle.mID];
	if (slot.generation != handle.mGeneration)
		return;

	//This invalidates all existing handles to the slot
	slot.generation++;
	mToBeDestroyed.push_back(handle.mID);
}

bool WallG::GameWorld::IsValid(GameObjectHandle handle) const
{
	return
		handle.mID >= 0 
			&& handle.mID < mGameObjects.size() 
			&& mGameObjects[handle.mID].generation == handle.mGeneration;
}

void WallG::GameWorld::ProcessDestroyList()
{
	ASSERT(!mUpdating, "GameWorld - Cannot Destory game object during update.");

	for (auto index : mToBeDestroyed)
	{
		auto& slot = mGameObjects[index];

		GameObject* gameObject = slot.gameobject.get();
		ASSERT(!IsValid(gameObject->GetHandle()), "GameObject is still valid");
		// Remove Game object to update list
		//Erase-Remove Idiom
		mUpdateList.erase(std::remove(mUpdateList.begin(),mUpdateList.end(), gameObject));
	
		//Terminate game object
		gameObject->Terminate();

		//Delete the game object and recycle the slot
		slot.gameobject.reset();
		mFreeSlots.push_back(index);
		
	}
	mToBeDestroyed.clear();
}
