#include "Precompiled.h"
#include "AIWorld.h"

using namespace WallG::AI;

namespace
{
	const float sCellSize = 100.0f;
}

void AIWorld::Initialize(const WallG::Math::Vector2& worldSize)
{
	//Ceilf make's 10.7 = 11;
	const auto numColumns = static_cast<uint32_t>(std::ceilf(worldSize.x / sCellSize));
	const auto numRows = static_cast<uint32_t>(std::ceilf(worldSize.y / sCellSize));
	mGrid.Initialize(numColumns, numRows);
}

void AIWorld::update()
{
	mGrid.ClearCells();
	for (auto entity: mEntities)
	{
		const auto column = static_cast<uint32_t>(entity->position.x / sCellSize);
		const auto row = static_cast<uint32_t>(entity->position.y / sCellSize);
		mGrid.GetCell(column,row).push_back(entity);
	}
}

void AIWorld::RegisterEntity(Entity* entity)
{
	mEntities.push_back(entity);
}

void AIWorld::UnregisterEntity(Entity* entity)
{
	auto iter = std::find(mEntities.begin(), mEntities.end(), entity);
	if (iter != mEntities.end())
	{
		std::iter_swap(iter, mEntities.end() - 1);
		mEntities.pop_back();
	}
}

void AIWorld::AddObstacle(const WallG::Math::Circle& obstacle)
{
	mObstacles.push_back(obstacle);
}

void AIWorld::AddWall(const WallG::Math::LineSegment& wall)
{
	mWalls.push_back(wall); 
}

void AIWorld::DebugDraw()
{
	for (auto& obstacle: mObstacles )
	{
		//WallG::DrawScreenCircle(obstacle.center, obstacle.radius, WallG::Colors::Blue);
	}
}

Agents AIWorld::GetNeighborhood(const WallG::Math::Circle& range, uint16_t typeId)
{
	Agents agents;

	int minX = static_cast<int>((range.center.x - range.radius) / sCellSize);
	int maxX = static_cast<int>((range.center.x + range.radius) / sCellSize);
	int minY = static_cast<int>((range.center.y - range.radius) / sCellSize);
	int maxY = static_cast<int>((range.center.y + range.radius) / sCellSize);
	
	minX = WallG::Math::Max(minX, 0);
	maxX = WallG::Math::Min(maxX, (int)mGrid.GetColumns() -1);
	minY = WallG::Math::Max(minY, 0);
	maxY = WallG::Math::Min(maxY, (int)mGrid.GetRows() - 1);

	for (int y = minY; y <= maxY; ++y)
	{
		for (int x = minX; x <= maxX; ++x)
		{
			auto& cell = mGrid.GetCell(x, y);
			for (auto entity: cell)
			{
				if (GetType(entity->id) == typeId)
				{
					agents.push_back(static_cast<Agent*>(entity));
				}
			}
		}
	}

	return agents;
}

int AIWorld::GetNextId(uint16_t typeId)
{
	// int_least32_t
	ASSERT(mNextId < UINT16_MAX, "Run out of ids!");
	const int id = (typeId << 16) | mNextId++;
	return id;
}

int AIWorld::GetType(int id) const
{
	return static_cast<uint16_t>(id >> 16);
}

