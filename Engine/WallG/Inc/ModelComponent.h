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

		void SetRotation(const Math::Quaternion rotation) { mRotation = rotation; }
		const Math::Quaternion& GetRotation() const { return mRotation; }
		
	protected:
		std::string mFileName;
		Graphics::ModelId mModelId = 0;
		Math::Quaternion mRotation;
	};
}
