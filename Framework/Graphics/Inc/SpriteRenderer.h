#pragma once
#include "TextureType.h"
#include "TextureManager.h"

namespace DirectX { class CommonStates; class SpriteBatch; }
using namespace WallG;

namespace WallG::Graphics
{
	class Texture;

	class SpriteRenderer
	{
	public:
		static void StaticInitialize();
		static void StaticTerminate();
		static SpriteRenderer* Get();

	public:
		SpriteRenderer();
		~SpriteRenderer();

		SpriteRenderer(const SpriteRenderer&) = delete;
		SpriteRenderer& operator=(const SpriteRenderer&) = delete;

		void Initialize();
		void Terminate();
		void Render();

		void SetTransform(const Math::Matrix4& transform);

		void BeginRender();
		void EndRender();

		void Draw(const Texture& texture, const Math::Vector2& pos, float rotation = 0.0f, Pivot pivot = Pivot::Center, Flip flip = Flip::None);
		void Draw(const Texture& texture, const Math::Rect& sourceRect, const Math::Vector2& pos, float rotation = 0.0f, Pivot pivot = Pivot::Center, Flip flip = Flip::None);

		void DrawSprite(TextureId textureId, const Math::Vector2& position, float rotation, Pivot pivot = Pivot::Center, Flip flip = Flip::None);
		void DrawSprite(TextureId textureId, const Math::Rect& sourceRect, const Math::Vector2& position);
		void DrawSprite(TextureId textureId, const Math::Rect& sourceRect, const Math::Vector2& position, Pivot pivot = Pivot::Center, Flip flip = Flip::None);
		void DrawSprite(TextureId textureId, const Math::Vector2& position, Pivot pivot = Pivot::Center, Flip flip = Flip::None);

		uint32_t GetSpriteWidth(TextureId textureId);
		uint32_t GetSpriteHeight(TextureId textureId);
		void* GetSprite(TextureId textureId);
	private:
		friend class Font;

		DirectX::CommonStates* mCommonStates;
		DirectX::SpriteBatch* mSpriteBatch;

		TextureId id = 0;
		Texture* texture = nullptr;
		Math::Matrix4 mTransform;
	};
}