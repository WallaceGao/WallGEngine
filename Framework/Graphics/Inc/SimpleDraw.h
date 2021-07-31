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

	void AddAABB(const Math::Vector3& center, const Math::Vector3& extend, const Color& color);
	void AddSphere(const Math::Vector3& center, float radius, const Color& color, uint32_t slices, uint32_t rings);

	void Render(const Camera& camera);
}