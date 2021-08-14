#pragma once
#include "DynamicUniverseTypes.h"
#include <WallG/Inc/WallG.h>

class TransformComponent;

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


protected:
	TransformComponent* mTransformComponent;


};


