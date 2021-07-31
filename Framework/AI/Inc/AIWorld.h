#pragma once
#include "Agent.h"
#include "PartitionGrid.h"

namespace WallG::AI
{
	class AIWorld
	{
	public:
		typedef std::vector<WallG::Math::Circle> Obstacles;
		typedef std::vector<WallG::Math::LineSegment> Walls;

		void Initialize(const WallG::Math::Vector2& worldSize);
		void update();

		void RegisterEntity(Entity* entity);
		void UnregisterEntity(Entity* entity);

		void AddObstacle(const WallG::Math::Circle& obstacle);
		void AddWall(const WallG::Math::LineSegment& wall);

		void DebugDraw();
		
		Agents GetNeighborhood(const WallG::Math::Circle& range, uint16_t typeId);
		int GetNextId(uint16_t typeId);
		int GetType(int id) const;

		const Obstacles& GetObstacles()const { return mObstacles; }
		const Walls& GetWalls()const { return mWalls; }
	private:
		Entities mEntities;
		int mNextId = 0;
		Obstacles mObstacles;
		Walls mWalls;
		PartitionGrid<Entity> mGrid;
	};
}