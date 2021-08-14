#pragma once

#include "Service.h"


namespace WallG
{
	class AnimatorComponent;
	class ModelComponent;
	class TransformComponent;

	class RenderService final : public Service
	{
	public:
		SET_TYPE_ID(ServiceId::Render)


		virtual void Initialize() override;
		virtual void Terminate()override;
		virtual void Update(float)override;
		virtual void Render() override;
		virtual void DebugUI()override;

		// Assign what Service need in to the function
		void Register(const ModelComponent* modelComponent);
		void Unregister(const ModelComponent* modelComponent);

	private:
		struct Entry
		{
			const AnimatorComponent* animatorComponent = nullptr;
			const ModelComponent* modelComponent = nullptr;
			const TransformComponent* transformComponent = nullptr;
		};

		struct TransformData
		{
			WallG::Math::Matrix4 world;
			WallG::Math::Matrix4 wvp;
			WallG::Math::Vector3 viewPostion;
			float padding;
		};

		struct Settings
		{
			float specularMapWeight = 1.0f;
			float bumpMapWeight = 0.2f;
			float normalMapWeight = 1.0f;
			int useShadow = 0;
			int useSkinning = 1;
			float padding[3];
		};

		struct StandardBoneTransformData
		{
			WallG::Math::Matrix4 boneTransform[256];
		};

		std::vector<Entry> mRenderEntries;

		WallG::Graphics::Camera mCamera;

		WallG::Graphics::TypedConstantBuffer<TransformData> mConstantBuffer;
		WallG::Graphics::TypedConstantBuffer<StandardBoneTransformData> mBoneTransformBuffer;
		WallG::Graphics::TypedConstantBuffer<Settings> mSettingsBuffer;
		WallG::Graphics::TypedConstantBuffer<WallG::Graphics::DirectionalLight> mLightBuffer;
		WallG::Graphics::TypedConstantBuffer<WallG::Graphics::Material> mMaterialBuffer;

		WallG::Graphics::DirectionalLight mDirctionalLight;
		WallG::Graphics::Material mMaterial;

		WallG::Graphics::VertexShader mVertexShader;
		WallG::Graphics::PixelShader mPixelShader;

		WallG::Graphics::Sampler mSampler;
		Settings mSettings;
	};
}