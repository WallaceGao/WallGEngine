#pragma once

#include "DynamicUniverseTypes.h"
#include <WallG/Inc/WallG.h>

class EconomicService;
class FactoryComponent final : public WallG::Component
{
public:
	SET_TYPE_ID(DUComponentId::Factory);
	MEMPOOL_DECLARE;

	FactoryComponent() = default;

	void Initialize() override;
	void Terminate() override;
	void Update(float deltaTime) override;
	void DebugUI() override;

	const float GetIronAmount() const { return mIronAmount; }
	const float GetIronCapacity() const { return mMaxIronCapacity; }
	const float GetCopperAmount() const { return mCopperAmount; }
	const float GetCopperCapacity() const { return mMaxCopperCapacity; }

	void SetMaxIronCapacity(float maxIronCapacity) { mMaxIronCapacity = maxIronCapacity; }
	void SetMaxCopperCapacity(float maxCopperCapacity) { mMaxCopperCapacity = maxCopperCapacity; }
	void SetEngineBuildTime(float engineBuildTime) { mEngineBuildTime = engineBuildTime; }
	void SetCargoBuildTime(float cargoBuildTime) { mCargoBuildTime = cargoBuildTime; }
	void SetMineToolBuildTime(float mineToolBuildTime) { mMineToolBuildTime = mineToolBuildTime; }

	
	// Need functions to sell items
	float SellItem(ItemType itemType);
	// Need functions to buy mineral
	float BuyMinral(MineralType minraType, float cargoSize);

protected:
	void SelectBestItemToProduce();

	int mEngineCount = 0;
	int mCargoCount = 0;
	int mMineToolCount = 0;
	int mEngineProduced = 0;
	int mCargoProduced = 0;
	int mMineToolProduced = 0;
	float mEngineBuildTime = 0.0f;
	float mCargoBuildTime = 0.0f;
	float mMineToolBuildTime = 0.0f;
	
	float mIronAmount = 0.0f;
	float mMaxIronCapacity = 0.0f;
	float mCopperAmount = 0.0f;
	float mMaxCopperCapacity = 0.0f;

	EconomicService* economicService;
	ItemType mCurrentProductionType = ItemType::None;
	float mProductionTime = 0.0f;
};


