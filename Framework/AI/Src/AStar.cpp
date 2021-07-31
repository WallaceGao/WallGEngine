#include "Precompiled.h"
#include "AStar.h"

using namespace WallG::AI;

bool AStar::Search(GridBasedGraph& graph, int startX, int startY, int endX, int endY, std::function<bool(int, int)> isBlocked, std::function<float(int, int, int, int)> getCost, std::function<float(int, int, int, int)> getDistence)
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
                if ((isBlocked(neighbor->column, neighbor->row) || neighbor->closed))
                {
                    continue;
                }
                //TODO:: Get undarstand 
                const float cost = nextNode->g + getCost(nextNode->column, nextNode->row, neighbor->column, neighbor->row);
                const float guess = getDistence(neighbor->column, neighbor->row, endNode->column, endNode->row);
                if ((!neighbor->opened))
                {
                    neighbor->opened = true;
                    mOpenList.push_back(neighbor);
                    //update the parent
                    neighbor->parent = nextNode;
                    neighbor->g = cost;
                    neighbor->h = guess;
                    std::list<GridBasedGraph::Node*>::iterator iter = mOpenList.begin();
                    for (; iter != mOpenList.end(); iter++)
                    {
                        if ((cost + neighbor->h) < (*iter)->g +(*iter)->h)
                        {
                            mOpenList.remove(neighbor);
                            mOpenList.insert(iter,neighbor);
                            break;
                        }
                    }
                }
                else if(cost < neighbor->g)
                {
                    neighbor->parent = nextNode;
                    neighbor->g = cost;
                    mOpenList.remove(neighbor);
                    std::list<GridBasedGraph::Node*>::iterator iter = mOpenList.begin();
                    for (; iter != mOpenList.end(); iter++)
                    {
                        if ((cost + neighbor->h) < (*iter)->g + (*iter)->h)
                        {
                            mOpenList.insert(iter, neighbor);
                            break;
                        }
                    }
                }
            }
        }
        nextNode->closed = true;
        mClosedList.push_back(nextNode);
    }
    return found;
}
