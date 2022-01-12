#include "Precompiled.h"
#include "EngineMath.h"

using namespace WallG::Math;

namespace 
{
	std::random_device randomDevice{};
	std::mt19937 randomGenerator{ randomDevice() };
}

int WallG::Math::Random::UniformInt(int min, int max)
{

	return std::uniform_int_distribution<>(min, max)(randomGenerator);
}

float Random::RandomFloat(float min, float max)
{
	return std::uniform_real_distribution<float>(min, max)(randomGenerator);
}

float Random::RandomNormal(float mean, float std)
{
	return std::normal_distribution<float>(mean, std)(randomGenerator);
}

Vector2 WallG::Math::Random::InsideUniteCirle()
{
	return Normalize(
		{
			RandomFloat(-1.0f,1.0f), 
			RandomFloat(-1.0f,1.0f)
	});
}

Vector2 WallG::Math::Random::OnUnitCircle()
{
	return Normalize(
		{
			RandomFloat(-1.0f,1.0f) + 0.001f, // ensure non-zero vector
			RandomFloat(-1.0f,1.0f)
		});
}

Vector3 WallG::Math::Random::InsideUniteSphere()
{
	return Normalize(
		{
			RandomFloat(-1.0f,1.0f),
			RandomFloat(-1.0f,1.0f),
			RandomFloat(-1.0f,1.0f)
		});
}

Vector3 WallG::Math::Random::OnUnitSphere()
{
	return Normalize(
		{
			RandomFloat(-1.0f,1.0f) + 0.001f,
			RandomFloat(-1.0f,1.0f),
			RandomFloat(-1.0f,1.0f)
		});
}