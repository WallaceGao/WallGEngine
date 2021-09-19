#include "FactoryService.h"

#include "EconomicService.h"
#include "FactoryComponent.h"

using namespace WallG;
using namespace WallG::Graphics;
using namespace WallG::Input;
using namespace WallG::Math;

const MineralType FactoryService::GetMostDesiredResource() const
{
  	auto economicService = GetWorld().GetService<EconomicService>();
	float IronInterestRate = economicService->GetInterestPrice(MineralType::Iron);
	float CopperInterestRate = economicService->GetInterestPrice(MineralType::Copper);

 	return  IronInterestRate > CopperInterestRate ? MineralType::Iron : MineralType::Copper ;
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
