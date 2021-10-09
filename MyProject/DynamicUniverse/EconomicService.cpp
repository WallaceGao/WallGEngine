#include "EconomicService.h"
#include <ImGui/Inc/ImPlot.h>

using namespace WallG;
using namespace WallG::Math;

namespace
{
	void PriceChange(float& price, float min, float max, float changeRate, bool increase)
	{
		//increase
		if (increase && price <= max)
		{
			price -= changeRate;
			if (price < min)
			{
				price = min;
			}

		}
		//decrease
		else if (!increase && price >= min)
		{
			price += changeRate;
			if (price > max)
			{
				price = max;
			}
		}
	}

	float CaculateInterestPrice(float unitPrice, float miningTime )
	{
		return unitPrice / miningTime;
	}
}

void EconomicService::Initialize()
{
	//iron

	//copper

	mIronPrice = mMaxIronPrice;
	mCopperPrice = mMaxCopperPrice;
}

void EconomicService::Update(float deltaTime)
{
	static float waitTime = 0.0f;
	waitTime -= deltaTime;
	if (waitTime <= 0.0f)
	{
		waitTime += 0.5f;
		float time = Core::TimeUtil::GetTime();
		mIronPoints.push_back({ time, mIronPrice });
		mCopperPoints.push_back({ time, mCopperPrice });
		mIronPerSecondPoints.push_back({ time,CaculateInterestPrice(mIronPrice, mIronMiningTime) });
		mCopperPerSecondPoints.push_back({ time,CaculateInterestPrice(mCopperPrice, mCopperMiningTime) });
	}
	if (mIronPoints.size() > 20000)
	{
		mIronPoints.erase(mIronPoints.begin());
		mCopperPoints.erase(mCopperPoints.begin());
		mIronPerSecondPoints.erase(mIronPerSecondPoints.begin());
		mCopperPerSecondPoints.erase(mCopperPerSecondPoints.begin());
	}
}

void EconomicService::DebugUI()
{
	ImGui::SetNextWindowSize(ImVec2{ 400, 600 });
	ImGui::Begin("Economic");
	{
		if (ImPlot::BeginPlot("Resouces", "X", "Y"))
		{
			ImPlot::PlotLine("Iron Price/ Cargo", &mIronPoints[0].x, &mIronPoints[0].y, static_cast<int>(mIronPoints.size()), 0, sizeof(Vector2));
			ImPlot::PlotLine("Copper Price/ Cargo", &mCopperPoints[0].x, &mCopperPoints[0].y, static_cast<int>(mCopperPoints.size()), 0, sizeof(Vector2));
			ImPlot::EndPlot();
		}
		if (ImPlot::BeginPlot("Items", "X", "Y"))
		{

			ImPlot::PlotLine("Iron Price/ Second", &mIronPerSecondPoints[0].x, &mIronPerSecondPoints[0].y, static_cast<int>(mIronPerSecondPoints.size()), 0, sizeof(Vector2));
			ImPlot::PlotLine("Copper Price/ Second", &mCopperPerSecondPoints[0].x, &mCopperPerSecondPoints[0].y, static_cast<int>(mCopperPerSecondPoints.size()), 0, sizeof(Vector2));
			ImPlot::EndPlot();
		}
	}
	ImGui::End();
}

const float EconomicService::ResourcePriceChange(MineralType minraType, float itemAmount, bool increase)
{
	if (minraType == MineralType::Iron)
	{
		//increase
		PriceChange(mIronPrice , mMinIronPrice, mMaxIronPrice, (itemAmount * 0.01f), increase);
		return mIronPrice;
	}
	else
	{
		//increase
		PriceChange(mCopperPrice, mMinCopperPrice, mMaxCopperPrice, (itemAmount * 0.01f), increase);
		return mCopperPrice;
	}
}

const float EconomicService::ItemPriceChange(ItemType itemType, bool increase)
{
	if (itemType == ItemType::Cargo)
	{
		//PriceChange(mCargoPrice, mMinCargoPrice, mMaxCargoPrice, 100, increase);
		return mCargoPrice;
	}
	else if (itemType == ItemType::MineTool)
	{
		//PriceChange(mMineToolPrice, mMinMineToolPrice, mMaxMineToolPrice, 100, increase);
		return mMineToolPrice;
	}
	else
	{
		//PriceChange(mEnginePrice, mMinEnginePrice, mMaxEnginePrice, 100, increase);
		return mEnginePrice;
	}
}

const float EconomicService::GetMiningTime(MineralType type)
{
	float miningTime = 0.0f;
	if (type == MineralType::Copper)
	{
		miningTime = mCopperMiningTime;
	}
	else
	{
		miningTime = mIronMiningTime;
	}
	return miningTime;
}

const float EconomicService::GetInterestPrice(MineralType type) const
{
	float interestPrice = 0.0f;
	if (type == MineralType::Iron)
	{
		interestPrice = CaculateInterestPrice(mIronPrice, mIronMiningTime);
	}
	else if (type == MineralType::Copper)
	{
		interestPrice = CaculateInterestPrice(mCopperPrice, mCopperMiningTime);
	}
	return interestPrice;
}


//float totalIronAmount = 0.0f;
//float totalIronCapacity = 0.0f;
//float totalCopperAmount = 0.0f;
//float totalCopperCapacity = 0.0f;
//
//for (auto factory : mFactoryEntries)
//{
//	totalIronAmount += factory.factoryComponent->GetIronAmount();
//	totalIronCapacity += factory.factoryComponent->GetIronCapacity();
//	totalCopperAmount += factory.factoryComponent->GetCopperAmount();
//	totalCopperCapacity += factory.factoryComponent->GetCopperCapacity();
//}
//
//float totalIronPercent = totalIronAmount / totalIronCapacity;
//float totalCopperPercent = totalCopperAmount / totalCopperCapacity;

