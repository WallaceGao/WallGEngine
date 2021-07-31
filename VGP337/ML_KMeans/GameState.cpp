#include "GameState.h"

#ifndef IMGUI_DEFINE_MATH_OPERATORS
#define IMGUI_DEFINE_MATH_OPERATORS
#endif

#include <ImGui/Inc/ImPlot.h>
#include <ImGui/Src/imgui_internal.h>

using namespace WallG::Graphics;
using namespace WallG::Input;
using namespace WallG::Math;
using namespace WallG::ML;


namespace
{
	struct ClusterResult
	{
		std::vector<Vector3> centroids;
		std::vector<int> assignment;
	};
	ClusterResult KMeansCluster(const std::vector<Vector3>& data, int k)
	{
		bool centroidChange = false;
		std::vector<Vector3> centroids;
		centroids.resize(k);
		//randomly initialize k centroids
		for (auto& centroid : centroids)
		{
			centroid.x = RandomFloat(0, 800.0f);
			centroid.y = RandomFloat(0, 600.0f);
		}

		//while centroids changed && # of iteration < max iteration:for point in dataset :
		std::vector<int> assignments;
		assignments.resize(data.size());
		while (!centroidChange)
		{
			//Do assignment here, by finding the closest centroid the datapoint belongs to
			for (size_t j = 0; j < data.size(); ++j)
			{
				// max number for float
				float closeDistence = FLT_MAX;
				for (size_t i = 0; i < centroids.size(); ++i)
				{
					float distance = DistanceSquared(centroids[i], data[j]);
					if (closeDistence > distance)
					{
						closeDistence = distance;
						assignments[j] = (int)i;
					}
				}
			}

			std::vector<Vector3> newCentroids;
			newCentroids.resize(k);
			int count = 0;

			for (size_t i = 0; i < k; ++i)
			{
				count = 0;
				for (size_t j = 0; j < assignments.size(); j++)
				{
					if (assignments[j] == i)
					{
						newCentroids[i] += data[j];
						count++;
					}
				}
				newCentroids[i] /= count;
			}

			for (size_t i = 0; i < newCentroids.size(); ++i)
			{
				if (newCentroids[i].x == centroids[i].x && newCentroids[i].y == centroids[i].y)
				{
					centroidChange = true;
				}
			}
			centroids = newCentroids;
		}
		ClusterResult result;
		result.assignment = assignments;
		result.centroids = centroids;
		return result;
	};

	ClusterResult clusterResult;
};

void GameState::Initialize()
{
	auto graphicsSystem = GraphicsSystem::Get();
	graphicsSystem->SetClearColor(Colors::Gray);

	mUnitTexture.Initialize(L"../../Assets/Images/interceptor_01.png");

	for (size_t i = 0; i < 50; ++i)
	{
		float x = RandomFloat(0.0f, 800.0f);
		float y = RandomFloat(0.0f, 600.0f);
		float vx = RandomFloat(-100.0f, 100.0f);
		float vy = RandomFloat(-100.0f, 100.0f);
		mUnits.emplace_back(Unit{ {x,y} , {vx,vy} });
	}
}

void GameState::Terminate()
{
	mUnitTexture.Terminate();
}

void GameState::Update(float deltaTime)
{
	auto inputSystem = InputSystem::Get();
	if (inputSystem->IsKeyPressed(KeyCode::ESCAPE))
	{
		WallG::MainApp().Quit();
		return;
	}

	if (inputSystem->IsKeyDown(KeyCode::SPACE))
	{
		std::vector<Vector3> data;
		int k = 3;
		for (size_t i = 0; i < mUnits.size(); i++)
		{
			//do not make cope can put it in
			data.emplace_back(mUnits[i].position.x, mUnits[i].position.y, 0);
		}
		clusterResult = KMeansCluster(data, k);
		for (size_t i = 0; i < clusterResult.assignment.size(); ++i)
		{
			mUnits[i].cluster = clusterResult.assignment[i];
		}
	}

	for (auto& unit : mUnits)
	{
		//unit.position += unit.velocity * deltaTime;
		if (unit.position.x < 0.0f)
		{
			unit.position.x += 800.0f;
		}
		if (unit.position.x > 800.0f)
		{
			unit.position.x -= 800.0f;
		}
		if (unit.position.y < 0.0f)
		{
			unit.position.y += 600.0f;
		}
		if (unit.position.y > 600.0f)
		{
			unit.position.y -= 600.0f;
		}
	}
}

void GameState::DebugUI()
{
	ImGui::SetNextWindowSize({ 800.0f,600.0f });
	ImGui::Begin("game", nullptr, ImGuiWindowFlags_NoResize);
	
	ImDrawList* drawList = ImGui::GetForegroundDrawList();
	ImVec2 winPos = ImGui::GetWindowPos();

	// ARGB 
	const ImU32 clusterColors[] = { 0xffff0000, 0xff00ff00, 0xff0000ff };
	for (auto& unit : mUnits)
	{
		ImGui::SetCursorPos(unit.position - ImVec2{ 32.0f, 32.0f });
		ImGui::Image(mUnitTexture.GetRawData(), { 64.0f, 64.0f });
		drawList->AddCircle(winPos + unit.position, 20.0f, clusterColors[unit.cluster]);
	}

	for (size_t i = 0; i < clusterResult.centroids.size(); ++i)
	{
		ImVec2 position;
		position.x = clusterResult.centroids[i].x;
		position.y = clusterResult.centroids[i].y;
		drawList->AddCircleFilled(winPos + position, 20.0f, clusterColors[i]);
	}
	

	ImGui::End();
}

