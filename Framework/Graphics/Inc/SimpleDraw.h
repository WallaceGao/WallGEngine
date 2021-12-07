#pragma once

#include "Colors.h"

namespace WallG::Graphics
{
	class Camera;
}

namespace WallG::Graphics::SimpleDraw
{
	void StaticInitialize(uint32_t maxVertexCount = 10000);
	void StaticTerminate();

	void AddLine(const Math::Vector3& v0, const Math::Vector3& v1, const Color& color);
	void AddLine2D(const Math::Vector2& v0, const Math::Vector2& v1, const Color& color);
	void AddLine2D(float x0, float y0, float x1, float y1, const Color& color);

	void AddCircle2D(const Math::Circle& circle, const Color& color);
	void AddCircle2D(const Math::Vector2& center, float r, const Color& color);

	void AddAABB(const Math::Vector3& center, const Math::Vector3& extend, const Color& color);
	void AddSphere(const Math::Vector3& center, float radius, const Color& color, uint32_t slices, uint32_t rings);
	void AddRing(const Math::Vector3& center, float radius, const Color& color, uint32_t slices = 32);

	void AddBone(const Math::Vector3& postion, const Math::Vector3& childPosition, const Color& color, const float scale, uint32_t numberOfLine);

	void Render(const Camera& camera);
}