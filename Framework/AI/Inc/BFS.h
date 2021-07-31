#pragma once
#include "GridBasedGraph.h"


namespace WallG::AI
{
	class BFS
	{
	public:
		bool Search(AI::GridBasedGraph& graph, int startX, int startY, int endX, int endY, std::function<bool(int, int)> isBlocked);
		const std::list<AI::GridBasedGraph::Node*>& GetClosedList() const { return mClosedList; }
	private:

		std::list<AI::GridBasedGraph::Node*> mOpenList;
		std::list<AI::GridBasedGraph::Node*> mClosedList;
	};
}

