#pragma once
#include "WallG/Inc/WallG.h"

class TileMap
{
public:
	static void StaticInitialize();
	static void StaticTerminate();
	static TileMap& Get();

public:
	void LoadTiles(std::filesystem::path fileName);
	void LoadMap(std::filesystem::path fileName);
	void SaveMap(std::filesystem::path fileName);

	void Resize(int columns, int rows);
	void changeTile();

	void Update();
	void Render() const;

	void ShowDebugUI();
	float traversalCost(int fromX, int fromY,int toX,int toY);
	float ManhattanDistence(int fromX, int fromY, int toX, int toY);
	float Euclidean(int fromX, int fromY, int toX, int toY);
	float Diagonal(int fromX, int fromY, int toX, int toY);

	int GetColumns() const { return mColumns; };
	int GetRows() const { return mRows; };
	const float GetTileHeight() const { return mTileHeight; };
	const float GetTileWidth() const { return mTileWidth; }
	bool IsBlocked(int columns, int rows) const;


private:
	std::vector<int> mMap;
	std::vector<WallG::Graphics::TextureId> mTiles;
	int mColumns = 0;
	int mRows = 0;
	float mTileWidth = 0;
	float mTileHeight = 0;
	int mTextureId = 1;
};
