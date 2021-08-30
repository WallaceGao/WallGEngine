#pragma once

#include "DynamicUniverseTypes.h"
#include <WallG/Inc/WallG.h>

class FactoryComponent final : public WallG::ModelComponent
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
	void SetIronPrice(float IronPrice) { mIronPrice = IronPrice; }
	void SetMinIronPrice(float MinIronPrice) { mMinIronPrice = MinIronPrice; }
	void SetMaxIronPrice(float MaxIronPrice) { mMaxIronPrice = MaxIronPrice; }
	void SetCopperPrice(float CopperPrice) { mCopperPrice = CopperPrice; }
	void SetMinCopperPrice(float MinCopperPrice) { mMinCopperPrice = MinCopperPrice; }
	void SetMaxCopperPrice(float MaxCopperPrice) { mMaxCopperPrice = MaxCopperPrice; }
	
	// Need functions to sell items
	void SellItem(ItemType itemType);
	// Need functions to buy mineral
	float BuyMinral(MineralType minraType, float cargoSize);

protected:
	void SelectBestItemToProduce();
	void PriceChange(MineralType minraType, float itemAmount, bool increase);

	int mEngineCount = 0;
	int mCargoCount = 0;
	int mMineToolCount = 0;
	int mEngineProduced = 0;
	int mCargoProduced = 0;
	int mMineToolProduced = 0;
	float mEngineBuildTime = 0.0f;
	float mCargoBuildTime = 0.0f;
	float mMineToolBuildTime = 0.0f;
	
	float mMinIronPrice = 0.0f;
	float mIronPrice = 0.0f;
	float mMaxIronPrice = 0.0f;
	float mIronAmount = 0.0f;
	float mMaxIronCapacity = 0.0f;
	float mMinCopperPrice = 0.0f;
	float mCopperPrice = 0.0f;
	float mMaxCopperPrice = 0.0f;
	float mCopperAmount = 0.0f;
	float mMaxCopperCapacity = 0.0f;


	ItemType mCurrentProductionType = ItemType::None;
	float mProductionTime = 0.0f;
};


