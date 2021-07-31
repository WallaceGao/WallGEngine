#pragma once
#include "ModelComponent.h"

namespace WallG
{
	class ShipComponent : public Component
	{
	public:
		SET_TYPE_ID(ComponentId::Model);
		MEMPOOL_DECLARE;
		enum Shader
		{
			Standard, Texturing
		};

		ShipComponent() = default;

		void Initialize() override;
		void Terminate() override;

		const Graphics::Model& GetModel() const;

		void SetFilePath(const char* fileName) { mFileName = fileName; }

	protected:
		std::string mFileName;

	};
}