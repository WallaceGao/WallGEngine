#pragma once

#include "DynamicUniverseTypes.h"
#include <WallG/Inc/WallG.h>

class EconomicService final : public WallG::Service
{
public:
	SET_TYPE_ID(DUServiceId::Economic);
	
	void Initialize() override;
	void Update(float deltaTime) override;
	void DebugUI() override;

	const float GetInterestPrice(MineralType type) const;

	const float ResourcePriceChange(MineralType minraType, float itemAmount, bool increase);
	const float ItemPriceChange(ItemType itemType, bool increase);

	const float GetMiningTime(MineralType Type);
private:

	struct Resouces
	{
		MineralType mType;
		float mMiningSpeed;
		float mInterestRate;
		float mUnitPrice;
		float mMaxUnitPrice;
		float mMinUnitPrice;
	};

	std::vector<Resouces> mResouces;

	float mIronPrice = 0.0f;
	float mMinIronPrice = 2.4f;
	float mMaxIronPrice = 5.6f;
	float mIronMiningTime = 4.0f;
	float mIronPricePerSecond = 0.0f;

	float mCopperPrice = 0.0f;
	float mMinCopperPrice = 3.6f;
	float mMaxCopperPrice = 8.4f;
	float mCopperMiningTime = 6.0f;
	float mCopperPricePerSecond = 0.0f;

	float mEnginePrice = 0.0f;
	float mMinEnginePrice = 1000.0f;
	float mMaxEnginePrice = 2000.0f;
	
	float mCargoPrice = 0.0f;
	float mMinCargoPrice = 1000.0f;
	float mMaxCargoPrice = 2000.0f;

	float mMineToolPrice = 0.0f;
	float mMinMineToolPrice = 1000.0f;
	float mMaxMineToolPrice = 2000.0f;

	std::vector<WallG::Math::Vector2> mIronPoints;
	std::vector<WallG::Math::Vector2> mCopperPoints;

	std::vector<WallG::Math::Vector2> mIronPerSecondPoints;
	std::vector<WallG::Math::Vector2> mCopperPerSecondPoints;


};