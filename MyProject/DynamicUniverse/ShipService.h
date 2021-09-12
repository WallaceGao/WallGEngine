#pragma once

#include "DynamicUniverseTypes.h"
#include <WallG/Inc/WallG.h>

class ShipComponent;
class UniverseService;

class ShipService final : public WallG::Service
{
public:
	SET_TYPE_ID(DUServiceId::Ship)

	void Initialize() override;
	void Update(float deltaTime) override;

	void Register(const ShipComponent* shipComponent);
	void Unregister(const ShipComponent* shipComponent);

private:
	const UniverseService* mUniverseService = nullptr;

	struct Entry
	{
		const ShipComponent* shipComponent;
	};
	int mNextId = 0;
	std::vector<Entry> mShipEntries;
};