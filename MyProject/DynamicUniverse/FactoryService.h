#pragma once

#include "DynamicUniverseTypes.h"
#include <WallG/Inc/WallG.h>

class FactoryComponent;

class FactoryService final : public WallG::Service
{
public:
	SET_TYPE_ID(DUServiceId::Factory)

	const MineralType GetMostDesiredResource() const;
	
	void Register(const FactoryComponent* factoryComponent);
	void Unregister(const FactoryComponent* factoryComponent);

private:
	
	struct Entry
	{
		const FactoryComponent* factoryComponent;
	};

	std::vector<Entry> mFactoryEntries;
};
