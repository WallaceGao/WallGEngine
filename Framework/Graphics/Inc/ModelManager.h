#pragma once
#include "Model.h"

namespace WallG::Graphics
{
	using ModelId = size_t;

	class ModelManager final
	{
	public:
		static void StaticInitialize();
		static void StaticTerminate();
		static ModelManager* Get();
		std::unique_ptr<ModelManager> mModelManager;

	public:
		void Terminate();

		ModelId LoadModel(std::filesystem::path filePath);
		ModelId NewEmptyModel(std::string name);

		const Model* GetModel(ModelId id) const;

	private:
		struct Entry
		{
			std::string name;
			std::unique_ptr<Model> model;
		};
		std::vector<Entry> mModels;
	};
}

