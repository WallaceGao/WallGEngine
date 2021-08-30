#include "FactoryComponent.h"

#include "FactoryService.h"

MEMPOOL_DEFINE(FactoryComponent, 1000)

void FactoryComponent::Initialize()
{
	auto factoryService = GetOwner().GetWorld().GetService<FactoryService>();
	factoryService->Register(this);
}

void FactoryComponent::Terminate()
{
	auto factoryService = GetOwner().GetWorld().GetService<FactoryService>();
	factoryService->Unregister(this);
}

void FactoryComponent::Update(float deltaTime)
{
	if (mCurrentProductionType == ItemType::None)
	{
		SelectBestItemToProduce();
	}
	else
	{
		mProductionTime -= deltaTime;
		if (mProductionTime <= 0.0f)
		{
			switch (mCurrentProductionType)
			{
			case ItemType::Engine:
				mEngineCount++;
				mEngineProduced++;
				if (mEngineProduced >= 100)
				{
					mEngineBuildTime -= 1.0f;
					mEngineProduced = 0;
				}
				mCurrentProductionType = ItemType::None;
				break;
			case ItemType::Cargo:
				mCargoCount++;
				mCargoProduced++;
				if (mCargoProduced >= 100)
				{
					mCargoBuildTime -= 1.0f;
					mCargoProduced = 0;
				}
				mCurrentProductionType = ItemType::None;
				break;
			case ItemType::MineTool:
				mMineToolCount++;
				mMineToolProduced++;
				if (mMineToolProduced >= 100)
				{
					mMineToolBuildTime -= 1.0f;
					mMineToolProduced = 0;
				}
				mCurrentProductionType = ItemType::None;
				break;
			}
		}
	}
}

void FactoryComponent::DebugUI()
{
	if (ImGui::CollapsingHeader("Factory", ImGuiTreeNodeFlags_DefaultOpen))
	{
		if (ImGui::CollapsingHeader("Resource", ImGuiTreeNodeFlags_DefaultOpen))
		{
			ImGui::Text("Iron: %f", mIronAmount);
			ImGui::Text("Price: %f", mIronPrice);
			ImGui::Text("Copper: %f", mCopperAmount);
			ImGui::Text("Price: %f", mCopperPrice);
		}
		if (ImGui::CollapsingHeader("Item", ImGuiTreeNodeFlags_DefaultOpen))
		{
			ImGui::Text("Engine: %d", mEngineCount);
			ImGui::Text("Cargo: %d", mCargoCount);
			ImGui::Text("MineTool: %d", mMineToolCount);
		}
		if (ImGui::Button("Add Iron"))
		{
			mIronAmount += 500;
			PriceChange(MineralType::Iron,500,true);
		}
		if (ImGui::Button("Add Copper"))
		{
			mCopperAmount += 500;
			PriceChange(MineralType::Copper, 500, true);
		}
	}
}

void FactoryComponent::SellItem(ItemType itemType)
{

}

float FactoryComponent::BuyMinral(MineralType minraType, float cargoSize)
{
	float wage = 0.0f;
	if (MineralType::Copper == minraType)
	{
		mCopperAmount += cargoSize;
		wage = cargoSize * mCopperPrice;
		PriceChange(MineralType::Iron, cargoSize, true);
	}
	else if (MineralType::Iron == minraType)
	{
		mIronAmount += cargoSize;
		wage = cargoSize * mIronPrice;
		PriceChange(MineralType::Iron, cargoSize, true);
	}
	return wage;
}

void FactoryComponent::SelectBestItemToProduce()
{
	if (mIronAmount >= 200 && mCopperAmount >= 200)
	{
		mProductionTime = mEngineBuildTime;
		mIronAmount -= 200;
		mCopperAmount -= 200;
		mCurrentProductionType = ItemType::Engine;
	}
	else if ( mCopperAmount >= 300)
	{
		mProductionTime = mCargoBuildTime;
		mCopperAmount -= 300;
		mCurrentProductionType = ItemType::Cargo;
	}
	else if ( mIronAmount >= 200)
	{
		mProductionTime = mMineToolBuildTime;
		mIronAmount -= 200;
		mCurrentProductionType = ItemType::MineTool;
	}
}

void FactoryComponent::PriceChange(MineralType minraType, float itemAmount, bool increase)
{
	if (minraType == MineralType::Iron)
	{
		//increase
		if (increase && mIronPrice < mMaxIronPrice)
		{
			mIronPrice += (itemAmount * 0.01f);
			if (mIronPrice > mMaxIronPrice)
			{
				mIronPrice = mMaxIronPrice;
			}
		}
		//decrease
		else if (!increase && mIronPrice > mMinIronPrice)
		{
			mIronPrice -= (itemAmount * 0.01f);
			if (mIronPrice < mMinIronPrice)
			{
				mIronPrice = mMinIronPrice;
			}
		}
	}
	else if (minraType == MineralType::Copper)
	{
		//increase
		if (increase && mCopperPrice < mMaxCopperPrice)
		{
			mCopperPrice += (itemAmount * 0.01f);
			if (mCopperPrice > mMaxCopperPrice)
			{
				mCopperPrice = mMaxCopperPrice;
			}
		}
		//decrease
		else if (!increase && mCopperPrice > mMinCopperPrice)
		{
			mCopperPrice -= (itemAmount * 0.01f);
			if (mCopperPrice < mMinCopperPrice)
			{
				mCopperPrice = mMinCopperPrice;
			}
		}
	}
}



