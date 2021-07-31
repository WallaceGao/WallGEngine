#include "GameState.h"
#include <ImGui/Inc/ImPlot.h>

using namespace WallG::Graphics;
using namespace WallG::Input;
using namespace WallG::Math;
using namespace WallG::ML;

namespace
{
	std::vector<Vector2> mCitylocation;
	float bestDistance = 0.0f;

	void GeneratePopupation(std::vector<GenetiAlgorithm::Genome>& Population, int chromoLength, int maxGeneValue)
	{
		for (auto& genome : Population)
		{
			genome.chromosome.reserve(chromoLength);
			std::vector<int> tempCities;
			for (int i = 0; i < chromoLength; i++)
				tempCities.push_back(i);
			for (int i = 0; i < chromoLength; ++i)
			{
				int indexToAdd = std::rand() % tempCities.size();
				genome.chromosome.push_back(tempCities[indexToAdd]);
				tempCities.erase(tempCities.begin() + indexToAdd);
			}
		}
	}

	float ComputeCityScore(const GenetiAlgorithm::Genome& genome)
	{
		float totalDistance = 0.0f;
		for (size_t i = 0; i < genome.chromosome.size() - 1; ++i)
		{
			totalDistance += Distance(mCitylocation[genome.chromosome[i]], mCitylocation[genome.chromosome[i + 1]]);
		}
		return bestDistance - totalDistance;
	}

	void ComputeCrossOverFunc(GenetiAlgorithm::Genome& offspring, const GenetiAlgorithm::Genome& parent1, const GenetiAlgorithm::Genome& parent2, float CrossoverRate)
	{
		for (size_t i = 0; i < parent1.chromosome.size(); i++)
		{
			offspring.chromosome.push_back(parent1.chromosome[i]);
		}

		// 0 2 [1¡¿ 3 4
		// 1 2 [4¡¿ 3 0

		// 1 becomes 4
		// 4 becomes 1

		while (RandomFloat() < CrossoverRate)
		{
			int indexToSwap = rand() % parent1.chromosome.size();
			int firstValueToSwap = parent1.chromosome[indexToSwap];
			int secondValueToSwap = parent2.chromosome[indexToSwap];

			for (size_t i = 0; i < offspring.chromosome.size(); ++i)
			{
				if (offspring.chromosome[i] == firstValueToSwap)
					offspring.chromosome[i] = secondValueToSwap;
				else if (offspring.chromosome[i] == secondValueToSwap)
					offspring.chromosome[i] = firstValueToSwap;
			}
		}
	}

	void ComputeMutationRateFunc(GenetiAlgorithm::Genome& offspring, const GenetiAlgorithm::Genome& parent1, float MutationRate, int mMaxGeneVlaue)
	{
		if (RandomFloat() < MutationRate)
		{
			auto index = std::rand() % offspring.chromosome.size();
			auto city = offspring.chromosome[index];
			offspring.chromosome.erase(offspring.chromosome.begin() + index);
			offspring.chromosome.push_back(city);
		}
	}
}

void GameState::Initialize()
{
	auto graphicsSystem = GraphicsSystem::Get();
	graphicsSystem->SetClearColor(Colors::Gray);
	mCitylocation.clear();

	//for(0+5)
	//x= sin(angle)* r
	//y = cos(angle) * r
	// angle + some = some

	mCitylocation.push_back(Vector2{ 200.0f , 200.0f });
	mCitylocation.push_back(Vector2{ 300.0f , 200.0f });
	mCitylocation.push_back(Vector2{ 400.0f , 200.0f });
	mCitylocation.push_back(Vector2{ 500.0f , 200.0f });
	mCitylocation.push_back(Vector2{ 500.0f , 400.0f });
	mCitylocation.push_back(Vector2{ 400.0f , 400.0f });
	mCitylocation.push_back(Vector2{ 300.0f , 400.0f });
	mCitylocation.push_back(Vector2{ 200.0f , 400.0f });

	for (size_t i = 0; i < mCitylocation.size() - 1; i++)
	{
		bestDistance += Distance(mCitylocation[i], mCitylocation[i + 1]);
	}
}

void GameState::Update(float deltaTime)
{
	auto inputSystem = InputSystem::Get();
	if (inputSystem->IsKeyPressed(KeyCode::ESCAPE))
	{
		WallG::MainApp().Quit();
		return;
	}

	if (mInitialized)
	{
		if (!mDone)
		{
			auto& best = mGA.GetBestGenome();
			if (best.fitness < 0.0f)
			{
				mGA.Advance();

				std::string bestCity;
				mAppLog.AddLog("Generation %d:", mGA.GetGeneration());
				for (auto& gene : mGA.GetBestGenome().chromosome)
				{
					mAppLog.AddLog("%s ", std::to_string(gene).c_str());
				}

				// A ranged-based for loop essentially expands to this:
				//{
				//    const auto& container = mGA.GetBestGenome().chromosome;
				//    for (auto iter = container.begin();
				//        iter != container.end();
				//        ++iter)
				//    {
				//        mAppLog.AddLog("%s ", std::to_string(gene).c_str());
				//    }
				//}

				mAppLog.AddLog("\n");
				mDone = mGA.GetBestGenome().fitness == 0.0f;
			}
		}
	}
}

void GameState::DebugUI()
{
	ImGui::Begin("controls", nullptr, ImGuiWindowFlags_AlwaysAutoResize);


	if (ImGui::Button("Run"))
	{
		mAppLog.AddLog("Initializeing new population ...\n");
		mGA.Initialize(100, (int)mCitylocation.size(), (int)mCitylocation.size(), 0.45f, 0.1f, GeneratePopupation, ComputeCityScore, ComputeCrossOverFunc, ComputeMutationRateFunc);
		mInitialized = true;
		mDone = false;
	}
	ImGui::End();

	mAppLog.Draw("console");
}






