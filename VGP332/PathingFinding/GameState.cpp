#include "GameState.h"
#include "WallG/Inc/WallG.h"

using namespace WallG;
using namespace WallG::AI;
using namespace WallG::Input;

namespace
{
	int GetIndex(int x, int y, int columns)
	{
		return x + (y * columns);
	}

	Math::Vector2 GetPosition(int column, int row, TileMap* tileMap)
	{
		return {
			column * tileMap->GetTileWidth() + tileMap->GetTileWidth() / 2 ,
			row * tileMap->GetTileHeight() + tileMap->GetTileWidth() / 2
		};
	}

	Math::Vector2 GetPosition(GridBasedGraph::Node* node, TileMap* tileMap)
	{
		return GetPosition(node->column, node->row, tileMap);
	}

}

//// Function passed as function pointer
//// bool (*)(int, int);
//bool IsBlock(int row, int column)
//{
//
//}
//
//// Function passed as functor (function object)
//struct IsBlock
//{
//	bool canUnitFly;
//	bool operator()(int row, int column)
//	{
//
//	}
//};

void GameState::Initialize()
{
	auto cameraService = mGameWorld.AddService<CameraService>();
	mGameWorld.AddService<RenderService>();
	mGameWorld.Initialize(1000);
	cameraService->SetCameraTo3D(false);

	tileMap.LoadTiles("TileSet_01.txt");
	tileMap.LoadMap("Map.txt");
	tileMap.StaticInitialize();
}

void GameState::Terminate()
{
	mGameWorld.Terminate();
	tileMap.StaticTerminate();
}

void GameState::Update(float deltaTime)
{
	auto inputSystem = Input::InputSystem::Get();
	tileMap.Update();

	if (graph.GetColumns() != tileMap.GetColumns() ||
		graph.GetRows() != tileMap.GetRows())
	{
		graph.Initialize(tileMap.GetColumns(), tileMap.GetRows());
		startX = 0;
		startY = 0;
		endX = 0;
		endY = 0;
		path.clear();
		closedList.clear();
	}
		
	if (foundPath)
	{
		// Got a new path, clear the old one!
		path.clear();

		// Beginning from the end node, trace back to it's parent one at a time
		auto node = graph.GetNode(endX, endY);
		while (node != nullptr)
		{
			path.push_back(node);
			node = node->parent;
		}

		// Once we recorded all the position from end to start, we need to reverse
		// them to get the correct order
		std::reverse(path.begin(), path.end());
	}

	// It is also helpful to draw the closed list
	for (auto iter = closedList.begin(); iter != closedList.end(); ++iter)
	{
		auto node = (*iter);
		if (node->parent != nullptr)
		{
			auto from = GetPosition(node,&tileMap);
			auto to = GetPosition(node->parent, &tileMap);
			SimpleDraw::AddLine2D(from, to, Colors::Blue);
		}
	}
}

void GameState::Render()
{
	// Grad graph
	tileMap.Render();

	float offset = tileMap.GetTileWidth() * 0.5f;
	float circleRadius = tileMap.GetTileWidth() * 0.2f;
	for (int y = 0; y < tileMap.GetRows(); y++)
	{
		for (int x = 0; x < tileMap.GetColumns(); x++)
		{
			const int index = GetIndex(x, y, tileMap.GetColumns());
			Math::Vector2 pos
			{
				static_cast<float>(x) * tileMap.GetTileWidth(),
				static_cast<float>(y) * tileMap.GetTileHeight()
			};
			if (mShowGraph)
			{
				SimpleDraw::AddCircle2D({ pos.x + offset , pos.y + offset }, circleRadius, Colors::DarkGray);
				std::array<GridBasedGraph::Node*, 8> coords = graph.GetNode(x, y)->neighbors;
				for (int i = 0; i < coords.size(); i++)
				{
					if (coords[i] != nullptr)
					{
						SimpleDraw::AddLine2D(pos.x + offset, pos.y + offset, coords[i]->column * tileMap.GetTileWidth() + offset, coords[i]->row * tileMap.GetTileHeight() + offset, Colors::Orange);
					}
				}
			}
		}
	}
	// Draw the path
	for (size_t i = 0; i + 1 < path.size(); ++i)
	{
		auto from = GetPosition(path[i], &tileMap);
		auto to = GetPosition(path[i + 1], &tileMap);
		SimpleDraw::AddLine2D(from, to, Colors::Cyan);
	}

	// Draw start and end
	for (float thickness = 0; thickness < 3; thickness += 1.0f)
	{
		float radius = 10.0f + thickness;
		SimpleDraw::AddCircle2D(GetPosition(startX, startY, &tileMap), radius, Colors::Magenta);
		SimpleDraw::AddCircle2D(GetPosition(endX, endY, &tileMap), radius, Colors::Red);
	}
	auto cameraService = mGameWorld.GetService<CameraService>();
	SimpleDraw::Render(cameraService->GetCamera());
}

void GameState::DebugUI()
{
	auto isBlocked = [this](int column, int row)
	{
		return tileMap.IsBlocked(column, row);
	};
	
	auto getCost = [this](int fromColumn, int fromRow, int toColumn, int toRow)
	{
		return tileMap.traversalCost(fromColumn, fromRow, toColumn, toRow);
	};
	
	auto getManhattanDistence = [this](int fromColumn, int fromRow, int toColumn, int toRow)
	{
		return tileMap.ManhattanDistence(fromColumn, fromRow, toColumn, toRow);
	};
	
	auto getEuclideanDistence = [this](int fromColumn, int fromRow, int toColumn, int toRow)
	{
		return tileMap.Euclidean(fromColumn, fromRow, toColumn, toRow);
	};
	
	auto getDiagonalDistence = [this](int fromColumn, int fromRow, int toColumn, int toRow)
	{
		return tileMap.Diagonal(fromColumn, fromRow, toColumn, toRow);
	};
	ImGui::Begin("Map Editor", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
	ImGui::Separator();
	ImGui::Checkbox("Show Grid", &mShowGraph);
	ImGui::SliderInt("Start X", &startX, 0, tileMap.GetColumns() - 1);
	ImGui::SliderInt("Start Y", &startY, 0, tileMap.GetRows() - 1);
	ImGui::SliderInt("End X", &endX, 0, tileMap.GetColumns() - 1);
	ImGui::SliderInt("End Y", &endY, 0, tileMap.GetRows() - 1);
	ImGui::Separator();
	if (ImGui::Button("Run BFS"))
	{
		BFS bfs;
		if (bfs.Search(graph, startX, startY, endX, endY, isBlocked))
		{
			foundPath = true;
			closedList = bfs.GetClosedList();
		}
	}
	if (ImGui::Button("Run DFS"))
	{
		DFS dfs;
		if (dfs.Search(graph, startX, startY, endX, endY, isBlocked))
		{
			foundPath = true;
			closedList = dfs.GetClosedList();
		}
	}
	if (ImGui::Button("Run Dijkstras"))
	{
		Dijkstras dijkstras;
		if (dijkstras.Search(graph, startX, startY, endX, endY, isBlocked, getCost))
		{
			foundPath = true;
			closedList = dijkstras.GetClosedList();
		}
	}
	if (ImGui::CollapsingHeader("A star", ImGuiTreeNodeFlags_DefaultOpen))
	{
		if (ImGui::RadioButton("Run Astar ManhattanDistence", &AstarList, 0))
		{
			AStar aStar;
			AstarList = 0;
			if (aStar.Search(graph, startX, startY, endX, endY, isBlocked, getCost, getManhattanDistence))
			{
				foundPath = true;
				closedList = aStar.GetClosedList();
			}
		}
		if (ImGui::RadioButton("Run Astar Euclidean Distence", &AstarList, 1))
		{
			AStar aStar;
			AstarList = 1;
			if (aStar.Search(graph, startX, startY, endX, endY, isBlocked, getCost, getEuclideanDistence))
			{
				foundPath = true;
				closedList = aStar.GetClosedList();
			}
		}
		if (ImGui::RadioButton("Run Astar Diagonal Distence", &AstarList, 2))
		{
			AStar aStar;
			AstarList = 2;
			if (aStar.Search(graph, startX, startY, endX, endY, isBlocked, getCost, getDiagonalDistence))
			{
				foundPath = true;
				closedList = aStar.GetClosedList();
			}
		}
	}
	ImGui::End();

	tileMap.ShowDebugUI();

}


