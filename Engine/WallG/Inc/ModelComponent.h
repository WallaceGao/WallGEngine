#pragma once
#include "Component.h"

namespace WallG
{
	class ModelComponent : public Component
	{
	public:
		SET_TYPE_ID(ComponentId::Model);
		MEMPOOL_DECLARE;
		enum Shader
		{
			Standard, Texturing
		};

		ModelComponent() = default;

		void Initialize() override;
		void Terminate() override;

		const Graphics::Model* GetModel() const;

		void SetFilePath(const char* fileName) { mFileName = fileName; }

	protected:
		std::string mFileName;
		Graphics::ModelId mModelId = 0;
	};
}
