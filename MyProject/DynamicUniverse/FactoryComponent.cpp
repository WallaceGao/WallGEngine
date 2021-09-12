#include "FactoryComponent.h"

#include "EconomicService.h"
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
		auto economicService = GetOwner().GetWorld().GetService<EconomicService>();
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
				economicService->ItemPriceChange(mCurrentProductionType, true);
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
				economicService->ItemPriceChange(mCurrentProductionType, true);
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
				economicService->ItemPriceChange(mCurrentProductionType, true);
				mCurrentProductionType = ItemType::None;
				break;
			}
		}
	}
}

void FactoryComponent::DebugUI()
{
	auto economicService = GetOwner().GetWorld().GetService<EconomicService>();
	if (ImGui::CollapsingHeader("Factory", ImGuiTreeNodeFlags_DefaultOpen))
	{
		if (ImGui::CollapsingHeader("Resource", ImGuiTreeNodeFlags_DefaultOpen))
		{
			ImGui::Text("Iron: %f", mIronAmount);
			ImGui::Text("Copper: %f", mCopperAmount);
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
			economicService->ResourcePriceChange(MineralType::Iron,500,true);
		}
		if (ImGui::Button("Add Copper"))
		{
			mCopperAmount += 500;
			economicService->ResourcePriceChange(MineralType::Copper, 500, true);
		}
	}
}

float FactoryComponent::SellItem(ItemType itemType)
{
	auto economicService = GetOwner().GetWorld().GetService<EconomicService>();
	return economicService->ItemPriceChange(itemType, false);
}

float FactoryComponent::BuyMinral(MineralType minraType, float cargoSize)
{
	auto economicService = GetOwner().GetWorld().GetService<EconomicService>();
	float wage = 0.0f;
	if (MineralType::Copper == minraType)
	{
		mCopperAmount += cargoSize;
		wage = economicService->ResourcePriceChange(MineralType::Iron, cargoSize, true);
	}
	else if (MineralType::Iron == minraType)
	{
		mIronAmount += cargoSize;
		wage = cargoSize * economicService->ResourcePriceChange(MineralType::Iron, cargoSize, true);
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
	else if ( mIronAmount >= 300)
	{
		mProductionTime = mMineToolBuildTime;
		mIronAmount -= 200;
		mCurrentProductionType = ItemType::MineTool;
	}
}





