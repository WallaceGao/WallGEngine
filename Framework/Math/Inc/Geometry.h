#pragma once
#include "Vector2.h"
#include "Vector3.h"

namespace WallG::Math
{
	struct Circle
	{
		Math::Vector2 center;
		float radius;

		Circle() : center(0.0f, 0.0f), radius(1.0f) {}
		Circle(const Math::Vector2& center, float radius) : center(center), radius(radius) {}
		Circle(float x, float y, float radius) : center(x, y), radius(radius) {}
	};

	struct LineSegment
	{
		Vector2 from;
		Vector2 to;

		LineSegment() : from(0.0f, 0.0f), to(0.0f, 0.0f) {}
		LineSegment(float x0, float y0, float x1, float y1) : from(x0, y0), to(x1, y1) {}
		LineSegment(const Vector2& v0, const Vector2& v1) : from(v0), to(v1) {}
	};

}
