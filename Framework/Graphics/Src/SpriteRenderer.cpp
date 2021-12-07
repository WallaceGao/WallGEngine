#include "Precompiled.h"
#include "SpriteRenderer.h"

#include "Colors.h"
#include "Camera.h"
#include "GraphicsSystem.h"
#include "Texture.h"
#include <DirectXTK/Inc/CommonStates.h>
#include <DirectXTK/Inc/SpriteBatch.h>

using namespace WallG::Math;
using namespace WallG::Graphics;


namespace
{
	struct SpriteCommand
	{
		SpriteCommand(TextureId inTextureId, const Math::Vector2& inPosiiton, float inRotation)
			: textureId(inTextureId)
			, sourceRect({ 0.0f, 0.0f, 0.0f, 0.0f })
			, position(inPosiiton)
			, rotation(inRotation)
		{}

		SpriteCommand(TextureId inTextureId, const Math::Vector2& inPosiiton, float inRotation, Pivot pivot, Flip flip)
			: textureId(inTextureId)
			, sourceRect({ 0.0f, 0.0f, 0.0f, 0.0f })
			, position(inPosiiton)
			, rotation(inRotation)
			, pivot(pivot)
			, flip(flip)
		{}

		SpriteCommand(TextureId inTextureId, const Math::Rect& inSourceRect, const Math::Vector2& inPosiiton, float inRotation)
			: textureId(inTextureId)
			, sourceRect(inSourceRect)
			, position(inPosiiton)
			, rotation(inRotation)
		{}

		SpriteCommand(TextureId inTextureId, const Math::Rect& inSourceRect, const Math::Vector2& inPosiiton, float inRotation, Pivot pivot, Flip flip)
			: textureId(inTextureId)
			, sourceRect(inSourceRect)
			, position(inPosiiton)
			, rotation(inRotation)
			, pivot(pivot)
			, flip(flip)
		{}

		TextureId textureId = 0;
		Math::Rect sourceRect{};
		Math::Vector2 position{ 0.0f };
		float rotation{ 0.0f };
		Pivot pivot = Pivot::Center;
		Flip flip = Flip::None;
	};

	struct TextCommand
	{
		TextCommand(std::wstring inStr, float inSize, float inX, float inY, uint32_t inColor)
			: str(std::move(inStr))
			, size(inSize)
			, x(inX)
			, y(inY)
			, color(inColor)
		{}

		std::wstring str;
		float size, x, y;
		uint32_t color;
	};

	HWND myWindow = nullptr;
	bool initialized = false;

	std::random_device myRandomDevice{};
	std::mt19937 myRandomEngine{ myRandomDevice() };

	Graphics::Color myBackgroundColor = { 0.1f, 0.1f, 0.1f, 1.0f };
	Graphics::Camera myCamera;
	//Font myFont;
	//Timer myTimer;
	float myZoom = 1.0f;

	//RenderTarget myRenderTarget;
	bool useRenderTarget = false;

	std::vector<SpriteCommand> mySpriteCommands;
	std::vector<TextCommand> myTextCommands;

	inline uint32_t ToColor(const Color& color)
	{
		uint8_t r = (uint8_t)(color.r * 255);
		uint8_t g = (uint8_t)(color.g * 255);
		uint8_t b = (uint8_t)(color.b * 255);
		return 0xff000000 | (b << 16) | (g << 8) | r;
	}
}

namespace
{
	DirectX::XMFLOAT2 GetOrigin(uint32_t width, uint32_t height, Pivot pivot)
	{
		auto index = static_cast<std::underlying_type_t<Pivot>>(pivot);
		constexpr DirectX::XMFLOAT2 offsets[] =
		{
			{ 0.0f, 0.0f }, // TopLeft
			{ 0.5f, 0.0f }, // Top
			{ 1.0f, 0.0f }, // TopRight
			{ 0.0f, 0.5f }, // Left
			{ 0.5f, 0.5f }, // Center
			{ 1.0f, 0.5f }, // Right
			{ 0.0f, 1.0f }, // BottomLeft
			{ 0.5f, 1.0f }, // Bottom
			{ 1.0f, 1.0f }, // BottomRight
		};
		return { width * offsets[index].x, height * offsets[index].y };
	}

	DirectX::SpriteEffects GetSpriteEffects(Flip flip)
	{
		switch (flip)
		{
		case Flip::Horizontal:
			return DirectX::SpriteEffects_FlipHorizontally;
		case Flip::Vertical:
			return DirectX::SpriteEffects_FlipVertically;
		case Flip::Both:
			return DirectX::SpriteEffects_FlipBoth;
		default:
			break;
		}
		return DirectX::SpriteEffects_None;
	}

	DirectX::XMFLOAT2 ToXMFLOAT2(const Math::Vector2& v)
	{
		return { v.x, v.y };
	}

	SpriteRenderer* sSpriteRenderer = nullptr;
}

void SpriteRenderer::StaticInitialize()
{
	ASSERT(sSpriteRenderer == nullptr, "[SpriteRenderer] System already initialized!");
	sSpriteRenderer = new SpriteRenderer();
	sSpriteRenderer->Initialize();
}

void SpriteRenderer::StaticTerminate()
{
	if (sSpriteRenderer != nullptr)
	{
		sSpriteRenderer->Terminate();
		delete sSpriteRenderer;
		sSpriteRenderer = nullptr;
	}
}

SpriteRenderer* SpriteRenderer::Get()
{
	ASSERT(sSpriteRenderer != nullptr, "[SpriteRenderer] No instance registered.");
	return sSpriteRenderer;
}

SpriteRenderer::SpriteRenderer()
	: mCommonStates(nullptr)
	, mSpriteBatch(nullptr)
{
}

SpriteRenderer::~SpriteRenderer()
{
	ASSERT(mSpriteBatch == nullptr, "[SpriteRenderer] Renderer not freed.");
}


void SpriteRenderer::Initialize()
{
	ASSERT(mSpriteBatch == nullptr, "[SpriteRenderer] Already initialized.");
	GraphicsSystem* gs = GraphicsSystem::Get();
	mCommonStates = new DirectX::CommonStates(gs->GetDevice());
	mSpriteBatch = new DirectX::SpriteBatch(gs->GetContext());
	initialized = true;
}

void SpriteRenderer::Terminate()
{
	ASSERT(mSpriteBatch != nullptr, "[SpriteRenderer] Already terminated.");
	delete mSpriteBatch;
	mSpriteBatch = nullptr;
	delete mCommonStates;
	mCommonStates = nullptr;
}

void WallG::Graphics::SpriteRenderer::Render()
{
	SpriteRenderer::Get()->BeginRender();
	for (const auto& command : mySpriteCommands)
	{
		if (id != command.textureId)
		{
			texture = TextureManager::Get()->GetTexture(command.textureId);
			id = command.textureId;
		}
		if (texture)
		{
			if (Math::IsEmpty(command.sourceRect))
			{
				SpriteRenderer::Get()->Draw(*texture, command.position, command.rotation, command.pivot, command.flip);
			}
			else
			{
				SpriteRenderer::Get()->Draw(*texture, command.sourceRect, command.position, command.rotation, command.pivot, command.flip);
			}
		}
	}
	mySpriteCommands.clear();
	SpriteRenderer::Get()->EndRender();
}

void SpriteRenderer::SetTransform(const Math::Matrix4& transform)
{
	mTransform = transform;
}

void SpriteRenderer::BeginRender()
{
	ASSERT(mSpriteBatch != nullptr, "[SpriteRenderer] Not initialized.");
	mSpriteBatch->Begin(
		DirectX::SpriteSortMode_Deferred,
		mCommonStates->NonPremultiplied(),
		nullptr,
		nullptr,
		nullptr,
		nullptr,
		DirectX::XMMATRIX(
			mTransform._11, mTransform._12, mTransform._13, mTransform._14,
			mTransform._21, mTransform._22, mTransform._23, mTransform._24,
			mTransform._31, mTransform._32, mTransform._33, mTransform._34,
			mTransform._41, mTransform._42, mTransform._43, mTransform._44
		));
}

void SpriteRenderer::EndRender()
{
	ASSERT(mSpriteBatch != nullptr, "[SpriteRenderer] Not initialized.");
	mSpriteBatch->End();

	// Restore state objects
	auto blendState = mCommonStates->Opaque();
	auto depthStencilState = mCommonStates->DepthDefault();
	auto rasterizerState = mCommonStates->CullCounterClockwise();

	ID3D11DeviceContext* context = GraphicsSystem::Get()->GetContext();
	context->OMSetBlendState(blendState, nullptr, 0xFFFFFFFF);
	context->OMSetDepthStencilState(depthStencilState, 0);
	context->RSSetState(rasterizerState);
}

void SpriteRenderer::Draw(const Texture& texture, const Math::Vector2& pos, float rotation, Pivot pivot, Flip flip)
{
	ASSERT(mSpriteBatch != nullptr, "[SpriteRenderer] Not initialized.");
	DirectX::XMFLOAT2 origin = GetOrigin(texture.GetWidth(), texture.GetHeight(), pivot);
	DirectX::SpriteEffects effects = GetSpriteEffects(flip);
	mSpriteBatch->Draw(texture.GetShaderResourceView(), ToXMFLOAT2(pos), nullptr, DirectX::Colors::White, rotation, origin, 1.0f, effects);
}

void SpriteRenderer::Draw(const Texture& texture, const Math::Rect& sourceRect, const Math::Vector2& pos, float rotation, Pivot pivot, Flip flip)
{
	ASSERT(mSpriteBatch != nullptr, "[SpriteRenderer] Not initialized.");
	RECT rect;
	rect.left = static_cast<LONG>(sourceRect.left);
	rect.top = static_cast<LONG>(sourceRect.top);
	rect.right = static_cast<LONG>(sourceRect.right);
	rect.bottom = static_cast<LONG>(sourceRect.bottom);
	DirectX::XMFLOAT2 origin = GetOrigin(rect.right - rect.left, rect.bottom - rect.top, pivot);
	DirectX::SpriteEffects effects = GetSpriteEffects(flip);
	mSpriteBatch->Draw(texture.GetShaderResourceView(), ToXMFLOAT2(pos), &rect, DirectX::Colors::White, rotation, origin, 1.0f, effects);
}

void WallG::Graphics::SpriteRenderer::DrawSprite(TextureId textureId, const Math::Vector2& position, float rotation, Pivot pivot, Flip flip)
{
	ASSERT(initialized, "WallG Engine not started.");
	mySpriteCommands.emplace_back(textureId, position, rotation, pivot, flip);
}

void WallG::Graphics::SpriteRenderer::DrawSprite(TextureId textureId, const Math::Rect& sourceRect, const Math::Vector2& position)
{
	ASSERT(initialized, "WallG Engine not started.");
	mySpriteCommands.emplace_back(textureId, sourceRect, position, 0.0f);
}

void WallG::Graphics::SpriteRenderer::DrawSprite(TextureId textureId, const Math::Rect& sourceRect, const Math::Vector2& position, Pivot pivot, Flip flip)
{
	ASSERT(initialized, "WallG Engine not started.");
	mySpriteCommands.emplace_back(textureId, sourceRect, position, 0.0f, pivot, flip);
}

void WallG::Graphics::SpriteRenderer::DrawSprite(TextureId textureId, const Math::Vector2& position, Pivot pivot, Flip flip)
{
	ASSERT(initialized, "WallG Engine not started.");
	mySpriteCommands.emplace_back(textureId, position, 0.0f, pivot, flip);
}

uint32_t WallG::Graphics::SpriteRenderer::GetSpriteWidth(TextureId textureId)
{
		Texture* texture = TextureManager::Get()->GetTexture(textureId);
		return texture ? texture->GetWidth() : 0u;
}

uint32_t WallG::Graphics::SpriteRenderer::GetSpriteHeight(TextureId textureId)
{
		Texture* texture = TextureManager::Get()->GetTexture(textureId);
		return texture ? texture->GetHeight() : 0u;
}

void* WallG::Graphics::SpriteRenderer::GetSprite(TextureId textureId)
{
		Texture* texture = TextureManager::Get()->GetTexture(textureId);
		return texture ? texture->GetShaderResourceView() : nullptr;
}
