#pragma once

#include "Service.h"

namespace WallG
{
	class SkyBoxService final : public Service
	{
	public:
		SET_TYPE_ID(ServiceId::SkyBox)

		virtual void Initialize() override;
		virtual void Terminate()override;

		virtual void Render() override;

	private:
		struct TransformData
		{
			WallG::Math::Matrix4 wvp;
		};

		WallG::Graphics::MeshPX mSkyboxMesh;
		WallG::Graphics::MeshBuffer mSkyboxMeshBuffer;
		WallG::Graphics::Texture mSkyboxTexture;

		WallG::Graphics::VertexShader mVertexShader;
		WallG::Graphics::PixelShader mPixelShader;

		WallG::Graphics::TypedConstantBuffer<TransformData> mConstantBuffer;

		WallG::Graphics::Sampler mSampler;
		WallG::Graphics::DepthStencilState mDisableDepthWrite;
	};
}