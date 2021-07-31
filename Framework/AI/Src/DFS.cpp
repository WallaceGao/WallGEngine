#include "Precompiled.h"
#include "DFS.h"

using namespace WallG::AI;

bool DFS::Search(GridBasedGraph& graph, int startX, int startY, int endX, int endY, std::function<bool(int, int)> isBlocked)
{
    graph.ResetSearchParams();
    // auto GridBasedGraph::Node*
    auto startNode = graph.GetNode(startX, startY);
    auto endNode = graph.GetNode(endX, endY);
    mOpenList.push_back(startNode);
    startNode->opened = true;

    bool found = false;
    while (!found && !mOpenList.empty())
    {
        // nextNode = get from open list
        GridBasedGraph::Node* nextNode;
        //fornt just give value
        nextNode = mOpenList.front();
        //remove frist value from list
        mOpenList.pop_front();

        // if (nextNode == end)
        //        found = true;
        // otherwise
        //        Follow the lecture slide
        if (nextNode == endNode)
        {
            found = true;
        }
        else
        {
            //expand node N
            for (auto neighbor : nextNode->neighbors)
            {
                if (neighbor == nullptr)
                {
                    continue;
                }
                if ((!neighbor->opened) && (!isBlocked(neighbor->column, neighbor->row)))
                {
                    neighbor->opened = true;
                    mOpenList.push_front(neighbor);
                    //update the parent
                    neighbor->parent = nextNode;
                }
            }
        }
        nextNode->closed = true;
        mClosedList.push_back(nextNode);
    }
    return found;
}
