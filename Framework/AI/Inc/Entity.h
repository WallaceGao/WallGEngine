#pragma once


namespace WallG::AI
{
	class AIWorld;

	class Entity
	{
	public:
		Entity(AIWorld& world, uint16_t typeId);
		virtual ~Entity();

		Entity(const Entity&) = delete;
		Entity(Entity&&) = delete;
		Entity& operator=(Entity&&) = delete;
		Entity& operator=(const Entity&) = delete;

		WallG::Math::Matrix3 LocalToWorld() const;

		AIWorld& world;
		WallG::Math::Vector2 position = WallG::Math::Vector2::Zero;
		WallG::Math::Vector2 heading = WallG::Math::Vector2::YAxis;
		const int id = 0;
		float radius = 1.0f;

	};

	using Entities = std::vector<Entity*>;
}