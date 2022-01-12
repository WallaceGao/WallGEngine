#pragma once

namespace WallG::Math::Random
{
	int UniformInt(int min = 0, int max = std::numeric_limits<int>::max());

	float RandomFloat(float min = 0.0f, float max = 1.0f);
	float RandomNormal(float mean, float  std);
	
	Vector2 InsideUniteCirle();
	Vector2 OnUnitCircle();

	Vector3 InsideUniteSphere();
	Vector3 OnUnitSphere();
}
