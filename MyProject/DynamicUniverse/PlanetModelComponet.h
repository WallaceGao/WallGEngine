#pragma once
#include "WallG/Inc/ModelComponent.h"
#include "WallG/Inc/GameObjectHandle.h"

namespace WallG
{
	class GameObjectHandle;
	class TransformComponent;

	class PlanetComponent final : public ModelComponent
	{
	public:
		SET_TYPE_ID(ComponentId::PlanetModel);
		MEMPOOL_DECLARE;

		PlanetComponent() = default;

		void Initialize() override;
		void Terminate() override;
		void Update(float deltaTime) override;
		void DebugUI() override;

		void SetParentPlanet(GameObjectHandle parentHandle) { mParentPlanet = parentHandle; }

		void SetTextureFilePath(const char* fileName) { mTextureFilePath = fileName; }
		
		void SetSelfRotation(const Math::Vector3& rotation) { mSelfRotation = rotation; }
		const Math::Vector3& GetSelfRotation() const { return mSelfRotation; }
		void SetSpeed(float speed) { mSpeed = speed; }
		const float GetSpeed() const { return mSpeed; }
		void SetSelfSpeed(float selfSpeed) { mSelfSpeed = selfSpeed; }
		const float GetSelfSpeed() const { return mSelfSpeed;}
		void SetPlanetScale(float planetScale) { mPlanetScale = planetScale; }
		void SetDistanceFromParent(float distance) { mDistanceFromParent = distance; }
		void SetName(std::string name) { mName = name; }
		const std::string GetName() const { return mName; }

		void Rota(float speed) { mSelfRotation -= speed; }

	protected:
		TransformComponent* mTransformComponent;
		GameObjectHandle mParentPlanet;
		Math::Vector3 mSelfRotation;
		Math::Vector3 mRotation;
		std::string mTextureFilePath;
		std::string mName;
		float mSpeed = 0.0f;
		float mSelfSpeed = 0.0f;
		float mPlanetScale = 1.0f;
		float mDistanceFromParent = 0.0f;
		bool mIsWorldCenter = false;

	};
};

