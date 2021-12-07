#pragma once
#include "TileMap.h"


class GameState : public WallG::AppState
{
public:
	void Initialize() override;
	void Terminate() override;

	void Update(float deltaTime) override;
	void Render() override;
	void DebugUI() override;



private:
	WallG::GameWorld mGameWorld;

	AI::GridBasedGraph graph;
	TileMap tileMap;
	std::vector<AI::GridBasedGraph::Node*> path;
	std::list<AI::GridBasedGraph::Node*> closedList;
	int startX = 0, startY = 0;
	int endX = 0, endY = 0;
	int AstarList = 0;
	bool mShowGraph = false;
	bool foundPath = false;
};