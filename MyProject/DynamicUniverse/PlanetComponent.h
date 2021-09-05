#pragma once

#include "DynamicUniverseTypes.h"
#include <WallG/Inc/WallG.h>

class GameObjectHandle;

class PlanetComponent final : public WallG::Component
{
public:
	SET_TYPE_ID(DUComponentId::Planet);
	MEMPOOL_DECLARE;

	PlanetComponent() = default;

	void Initialize() override;
	void Terminate() override;
	void Update(float deltaTime) override;
	void DebugUI() override;

	void SetParentPlanet(WallG::GameObjectHandle parentHandle) { mParentPlanet = parentHandle; }

	void SetTextureFilePath(const char* fileName) { mTextureFilePath = fileName; }
	
	void SetSelfRotation(const WallG::Math::Vector3& rotation) { mSelfRotation = rotation; }
	void SetSpeed(float speed) { mSpeed = speed; }
	void SetSelfSpeed(float selfSpeed) { mSelfSpeed = selfSpeed; }
	void SetDistanceFromParent(const WallG::Math::Vector3& distance) { mDistanceFromParent = distance; }
	void SetName(std::string name) { mName = name; }
	void SetResourceType(int resourceType) { mResourceType = MineralType(resourceType); }
	const WallG::Math::Vector3& GetSelfRotation() const { return mSelfRotation; }
	const float GetSpeed() const { return mSpeed; }
	const std::string GetName() const { return mName; }
	const float GetSelfSpeed() const { return mSelfSpeed;}
	const MineralType GetResourceType() const { return mResourceType; }

	void Rota(float speed) { mSelfRotation -= speed; }

protected:
	WallG::TransformComponent* mTransformComponent;
	WallG::GameObjectHandle mParentPlanet;
	WallG::Math::Vector3 mSelfRotation;
	WallG::Math::Vector3 mRotation;
	std::string mTextureFilePath;
	std::string mName;
	float mSpeed = 0.0f;
	float mSelfSpeed = 0.0f;
	WallG::Math::Vector3 mDistanceFromParent = WallG::Math::Vector3::Zero;

	MineralType mResourceType = MineralType::None;
};


