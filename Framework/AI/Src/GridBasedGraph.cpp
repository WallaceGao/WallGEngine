#include "Precompiled.h"
#include "GridBasedGraph.h"

using namespace WallG::AI;

namespace 
{
	inline int GetIndex(int x, int y, int columns)
	{
		return x + (y * columns);
	}
}

void GridBasedGraph::Initialize(int columns, int rows)
{
	// mNode.resize
	// for(all rows)
	// for (all columns)
	//    n = GetNode(columns, row)
	//     for (all direction)
	//        n addneighbor(GetNode(x,y))

	mColumns = columns;
	mRows = rows;
	mNodes.resize(columns * rows);

	for (int j = 0; j < rows; ++j)
	{
		for (int i = 0; i < columns; ++i)
		{
			mNodes[GetIndex(i, j, mColumns)].column = i;
			mNodes[GetIndex(i, j, mColumns)].row = j;
			mNodes[GetIndex(i, j, mColumns)].neighbors[West] = GetNode(i - 1,j);
			mNodes[GetIndex(i, j, mColumns)].neighbors[East] = GetNode(i + 1, j);
			mNodes[GetIndex(i, j, mColumns)].neighbors[North] = GetNode(i, j - 1);
			mNodes[GetIndex(i, j, mColumns)].neighbors[South] = GetNode(i, j + 1);
			mNodes[GetIndex(i, j, mColumns)].neighbors[NorthEast] = GetNode(i + 1, j - 1);
			mNodes[GetIndex(i, j, mColumns)].neighbors[NorthWest] = GetNode(i - 1, j - 1);
			mNodes[GetIndex(i, j, mColumns)].neighbors[SouthEast] = GetNode(i + 1, j + 1);
			mNodes[GetIndex(i, j, mColumns)].neighbors[SouthWest] = GetNode(i - 1, j + 1);
		}
	}



	//for (int i = 0; i < rows; i++)
	//{
	//	for (int j = 0; j < columns; j++)
	//	{
	//		// left
	//		if (columns != 0)
	//		{
	//			mNodes[columns + (rows * mColumns)].neighbors[West]->column = columns - 1;
	//			mNodes[columns + (rows * mColumns)].neighbors[West]->row = rows;
	//		}
	//		// right
	//		if (columns != mColumns - 1)
	//		{
	//			mNodes[columns + (rows * mColumns)].neighbors[East]->column = columns + 1;
	//			mNodes[columns + (rows * mColumns)].neighbors[East]->row = rows;
	//		}
	//		//UP
	//		if( rows != 0)
	//		{
	//			mNodes[columns + (rows * mColumns)].neighbors[North]->column = columns;
	//			mNodes[columns + (rows * mColumns)].neighbors[North]->row = rows -1;
	//		}
	//		//down
	//		if( rows != mRows -1)
	//		{
	//			mNodes[columns + (rows * mColumns)].neighbors[South]->column = columns;
	//			mNodes[columns + (rows * mColumns)].neighbors[South]->row = rows + 1;
	//		}
	//		//UpLeft
	//		if (columns != 0 && rows != 0)
	//		{
	//			mNodes[columns + (rows * mColumns)].neighbors[NorthWest]->column = columns - 1;
	//			mNodes[columns + (rows * mColumns)].neighbors[NorthWest]->row = rows - 1;
	//		}
	//		if (columns != 0 && rows != mRows - 1)
	//		{
	//			mNodes[columns + (rows * mColumns)].neighbors[NorthEast]->column = columns - 1;
	//			mNodes[columns + (rows * mColumns)].neighbors[NorthEast]->row = rows +1;
	//		}
	//		if (columns != mColumns - 1 && rows != 0)
	//		{
	//			mNodes[columns + (rows * mColumns)].neighbors[SouthWest]->column = columns +1;
	//			mNodes[columns + (rows * mColumns)].neighbors[SouthWest]->row = rows -1;
	//		}
	//		if (columns != mColumns - 1 && rows != mRows - 1)
	//		{
	//			mNodes[columns + (rows * mColumns)].neighbors[NorthEast]->column = columns + 1;
	//			mNodes[columns + (rows * mColumns)].neighbors[NorthEast]->row = rows + 1;
	//		}
	//	}
	//}
}

void GridBasedGraph::ResetSearchParams()
{
	for (int i = 0; i < mNodes.size(); i++)
	{
		mNodes[i].closed = false;
		mNodes[i].opened = false;
		mNodes[i].parent = nullptr;
		mNodes[i].g = 0.0f;
		mNodes[i].h = 0.0f;
	}
}

