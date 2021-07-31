#pragma once

namespace WallG::AI
{
    class GridBasedGraph
    {
    public:
        enum Direction
        {
            North,
            South,
            East,
            West,
            NorthEast,
            NorthWest,
            SouthEast,
            SouthWest
        };

        struct Node
        {
            //Graph structre- set in initialize;
            std::array<Node*, 8> neighbors;
            int column = 0;
            int row = 0;
            float g = 0.0f;
            float h = 0.0f;

            //Search Params
            Node* parent = nullptr;
            bool opened = false;
            bool closed = false;
        };

        void Initialize(int columns, int rows);
        void ResetSearchParams();

        //TODO: Add Bound Check here, if out of bound return null ;

        Node* GetNode(int column, int row) 
        { 
            if (column >= mColumns || column < 0 || row >= mRows || row < 0)
            {
                return nullptr;
            }
            return& mNodes[column + (row * mColumns)]; 
        }
        const Node* GetNode(int column, int row) const 
        { 
            if (column >= mColumns || column < 0 || row >= mRows || row < 0)
            {
                return nullptr;
            }
            return& mNodes[column + (row * mColumns)]; 
        }

        int GetColumns() const { return mColumns; };
        int GetRows() const { return mRows; };

    private:
        std::vector<Node> mNodes;
        int mColumns = 0;
        int mRows = 0;
    };
}


