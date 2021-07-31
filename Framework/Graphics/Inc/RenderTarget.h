#pragma once
#include "Colors.h"
#include "Texture.h"

namespace WallG::Graphics
{
	class RenderTarget final : public Texture
	{
	public: 
		static void UnbindPS(uint32_t slot);
	public:
		RenderTarget() = default;
		~RenderTarget() override;

		void Initialize(const std::filesystem::path&) override;
		void Initialize(uint32_t width, uint32_t height, Format format = Format::RGBA_U8) override;
		void Terminate() override;

		void BeginRender(Color clearColor = Colors::Black) const;
		void EndRender() const;
	public:
		ID3D11RenderTargetView* mRenderTargetView = nullptr;
		ID3D11DepthStencilView* mDepthStencilView = nullptr;
		D3D11_VIEWPORT mViewport;
		D3D11_MAPPED_SUBRESOURCE mSubresource;
	};
}