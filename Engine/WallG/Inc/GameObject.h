#pragma once

#include "Component.h"
#include "GameObjectHandle.h"

namespace WallG
{
	class GameWorld;

	class GameObject final
	{
	public:
		GameObject() = default;
		MEMPOOL_DECLARE;

		void Initialize();
		void Terminate();
		void Update(float deltaTime);
		void DebugUI();

		void AddComponent(Component* newComponent);

		template<class ComponentType>
		ComponentType* AddComponent()
		{
			ASSERT(!mInitialized, "GameObject - Can only add component before object");
			auto& newComponent = mComponets.emplace_back(std::make_unique<ComponentType>());
			newComponent->mOwner = this;
			return static_cast<ComponentType*>(newComponent.get());
		}

		template<class ComponetType>
		ComponetType* GetComponent()
		{
			auto constMe = static_cast<const GameObject*>(this);
			return const_cast<ComponetType*>(constMe->GetComponent<ComponetType>());
		}

		template <class ComponentType>
		const ComponentType* GetComponent() const
		{
			for (auto& component : mComponets)
			{
				if (component->GetTypeId() == ComponentType::StaticGetTypeId())
				{
					return static_cast<ComponentType*>(component.get());
				}
			}
			return nullptr;
		}


		GameWorld& GetWorld() { return *mWorld; }
		const GameWorld& GetWorld() const { return *mWorld; }

		GameObjectHandle GetHandle() const { return mHandle; }

		void SetName(const char* name) { mName = name; }
		const std::string& GetName() const { return mName; }

	private:
		friend class GameWorld;

		using Components = std::vector<std::unique_ptr<Component>>;

		GameWorld* mWorld = nullptr;
		Components mComponets;
		GameObjectHandle mHandle;
		std::string mName = "NoName";
		bool mInitialized = false;

	};
}
