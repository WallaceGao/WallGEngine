#pragma once

#include "GameObject.h"
#include "Service.h"

namespace WallG
{
	class GameWorld final
	{
	public:
		void Initialize(uint32_t capacity);
		void Terminate();

		void Update(float deltaTime);
		void Render();
		void DebugUI();

		
		//TODO Add template class
		template<class ServiceType>
		ServiceType* AddService()
		{
			ASSERT(!mInitialized, "GameWorld - Can only add service before world is initialized");
			auto& newService = mServices.emplace_back(std::make_unique<ServiceType>());
			newService->mWorld = this;
			return static_cast<ServiceType*>(newService.get());
		}

		template<class ServiceType>
		ServiceType* GetService()
		{
			for (auto& service : mServices)
			{
				if (service->GetTypeId() == ServiceType::StaticGetTypeId())
				{
					return static_cast<ServiceType*>(service.get());
				}
			}
			return nullptr;
		}

		GameObject* CreatGameObject(const std::filesystem::path& filePath, std::string name);
		GameObject* GetGameObject(GameObjectHandle handle);
		GameObject* FindGameObject(const std::string& name);
		void Destroy(GameObjectHandle handle); 

	private:
		bool IsValid(GameObjectHandle handle) const;
		void ProcessDestroyList();

		struct  Slot
		{
			std::unique_ptr<GameObject> gameobject;
			uint32_t generation = 0;
		};

		using GameObjectsSlot = std::vector<Slot>;
		using GameObjectList = std::vector<GameObject*>;
		using Service = std::vector<std::unique_ptr<Service>>;

		Service mServices;
		GameObjectsSlot mGameObjects;
		std::vector<uint32_t> mFreeSlots;
		std::vector<uint32_t> mToBeDestroyed;

		GameObjectList mUpdateList;
		GameObject* mSelectedObject = nullptr;

		bool mInitialized;
		bool mUpdating;
	};
}
