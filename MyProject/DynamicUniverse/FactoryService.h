#pragma once

#include "DynamicUniverseTypes.h"
#include <WallG/Inc/WallG.h>

namespace WallG
{
	class FactoryService final : public Service
	{
	public:
		SET_TYPE_ID(DUServiceId::Factory)

		void Initialize() override;
		void Terminate() override;
		void Update(float deltaTime) override;
		void DebugUI() override;

		const MineralType GetMostDesiredResource() const;
	
	private:
		float mFPS = 0.0f;

		std::vector<float> mPlanetScale;
	};
}