#pragma once
#include "Component.h"

namespace WallG
{
	class MaterialComponent final : public Component
	{
	public:
		SET_TYPE_ID(ComponentId::Material);
		MEMPOOL_DECLARE;

		MaterialComponent() = default;

		void Initialize() override;
		void Terminate() override;


	private:
		WallG::Graphics::Material mMaterial;
		WallG::Graphics::TypedConstantBuffer<WallG::Graphics::Material> mMaterialBuffer;

	};
}
