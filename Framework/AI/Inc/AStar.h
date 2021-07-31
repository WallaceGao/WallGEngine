#pragma once
#include "GridBasedGraph.h"

namespace WallG::AI
{
	class AStar
	{
	public:
		bool Search(GridBasedGraph& graph, int startX, int startY, int endX, int endY, std::function<bool(int, int)> isBlocked, std::function<float(int, int, int, int)> getCost, std::function<float(int, int, int, int)> getDistence);
		const std::list<GridBasedGraph::Node*>& GetClosedList() const { return mClosedList; }
	private:

		std::list<GridBasedGraph::Node*> mOpenList;
		std::list<GridBasedGraph::Node*> mClosedList;
	};
}