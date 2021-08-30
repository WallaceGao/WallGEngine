#include "FactoryService.h"

#include "FactoryComponent.h"

using namespace WallG;
using namespace WallG::Graphics;
using namespace WallG::Input;
using namespace WallG::Math;

const MineralType FactoryService::GetMostDesiredResource() const
{
	float totalIronAmount = 0.0f;
	float totalIronCapacity = 0.0f;
	float totalCopperAmount = 0.0f;
	float totalCopperCapacity = 0.0f;

	MineralType needResource;
	for (auto factory: mFactoryEntries)
	{
		totalIronAmount += factory.factoryComponent->GetIronAmount();
		totalIronCapacity += factory.factoryComponent->GetIronCapacity();
		totalCopperAmount += factory.factoryComponent->GetCopperAmount();
		totalCopperCapacity += factory.factoryComponent->GetCopperCapacity();

		//if (factory.factoryComponent->GetIronAmount() >= factory.factoryComponent->GetCopperAmount())
		//{
		//	needResource = MineralType::Copper;
		//}
		//else
		//{
		//	needResource = MineralType::Iron;
		//}
	}

	float totalIronPercent = totalIronAmount / totalIronCapacity;
	float totalCopperPercent = totalCopperAmount / totalCopperCapacity;

	return  totalIronPercent > totalCopperPercent? MineralType::Copper : MineralType::Iron ;
}

void FactoryService::Register(const FactoryComponent* factoryComponent)
{
	const GameObject& factory = factoryComponent->GetOwner();
	Entry& entry = mFactoryEntries.emplace_back();
	entry.factoryComponent = factoryComponent;
}

void FactoryService::Unregister(const FactoryComponent* factoryComponent)
{
	auto match = [&](const auto& entry) {return (entry.factoryComponent == factoryComponent); };
	auto iter = std::find_if(mFactoryEntries.begin(), mFactoryEntries.end(), match);
	if (iter != mFactoryEntries.end())
	{
		mFactoryEntries.erase(iter);
	}
}
