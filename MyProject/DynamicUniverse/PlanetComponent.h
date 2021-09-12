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
	const WallG::Math::Vector3& GetSelfRotation() const { return mSelfRotation; }
	void SetSpeed(float speed) { mSpeed = speed; }
	const float GetSpeed() const { return mSpeed; }
	void SetSelfSpeed(float selfSpeed) { mSelfSpeed = selfSpeed; }
	const float GetSelfSpeed() const { return mSelfSpeed;}
	void SetDistanceFromParent(float distance) { mDistanceFromParent = distance; }
	const MineralType GetResourceType() const { return mResourceType; }
	void SetResourceType(int resourceType) { mResourceType = MineralType(resourceType); }
	const WallG::Math::Vector3 GetPlanetScale() const { return mPlanetScale; }
	void SetPlanetScale(const WallG::Math::Vector3& planetScale) { mPlanetScale = planetScale; }
	void SetStartRota(const float rota) { mStartRota = rota; }

protected:

	WallG::TransformComponent* mTransformComponent;
	WallG::GameObjectHandle mParentPlanet;
	WallG::Math::Vector3 mPlanetScale = WallG::Math::Vector3::Zero;
	WallG::Math::Vector3 mSelfRotation = WallG::Math::Vector3::Zero;
	WallG::Math::Vector3 mRotation = WallG::Math::Vector3::Zero;
	std::string mTextureFilePath;
	float mSpeed = 0.0f;
	float mSelfSpeed = 0.0f;
	float mStartRota = 0.0f;
	float mDistanceFromParent = 0.0f;

	MineralType mResourceType = MineralType::None;
};


