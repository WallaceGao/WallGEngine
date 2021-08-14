#pragma once

#include "DynamicUniverseTypes.h"
#include <WallG/Inc/WallG.h>

class GameObjectHandle;

class PlanetComponent final : public WallG::ModelComponent
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
	void SetPlanetScale(float planetScale) { mPlanetScale = planetScale; }
	void SetDistanceFromParent(float distance) { mDistanceFromParent = distance; }
	void SetName(std::string name) { mName = name; }
	const std::string GetName() const { return mName; }

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
	float mPlanetScale = 1.0f;
	float mDistanceFromParent = 0.0f;
	bool mIsWorldCenter = false;

};


