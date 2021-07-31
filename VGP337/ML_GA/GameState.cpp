#include "GameState.h"
#include <ImGui/Inc/ImPlot.h>

using namespace WallG::Graphics;
using namespace WallG::Input;
using namespace WallG::Math;
using namespace WallG::ML;

namespace
{
    const std::string kTarget = "Genetic Algorithm is runnnn!";
    const int kValidGeneValue = 126 - 32; // form "~" to " space"

    void GeneratePopupation(std::vector<GenetiAlgorithm::Genome>& Population, int chromoLength, int maxGeneValue)
    {
        for (auto& genome : Population)
        {
        	//resuze the vector but not push anything on it.
        	genome.chromosome.reserve(chromoLength);
        	for (int i = 0; i < chromoLength; ++i)
        	{
        		genome.chromosome.push_back(std::rand() % maxGeneValue + 0);
        	}
        }
    }

    float ComputeStringScore(const GenetiAlgorithm::Genome& genome)
    {
        float fitness = 0.0f;
        for (size_t i = 0; i < kTarget.size(); ++i)
        {
            if (kTarget[i] == genome.chromosome[i] + 32)
            {
                fitness += 1.0f;
            }
        }
        return fitness;
    };

    void ComputeCrossOverFunc(GenetiAlgorithm::Genome& offspring,const GenetiAlgorithm::Genome& parent1, const GenetiAlgorithm::Genome& parent2, float mMutationRate)
    {
        for (size_t i = 0; i < parent1.chromosome.size(); ++i)
        {
            if (RandomFloat() < mMutationRate)
            {
                offspring.chromosome.push_back(parent1.chromosome[i]);
            }
            else
            {
                offspring.chromosome.push_back(parent2.chromosome[i]);
            }
        }
    }

    void ComputeMutationRateFunc(GenetiAlgorithm::Genome& offspring, const GenetiAlgorithm::Genome& parent1, float MutationRate, int mMaxGeneVlaue)
    {
           //int num = (int)RandomFloat(0, offspring.chromosome.size() - 1);
           //int num2 = (int)RandomFloat(0, offspring.chromosome.size() - 1);
           //offspring.chromosome[num] = std::rand() % mMaxGeneVlaue + 0;
           //offspring.chromosome[num2] = std::rand() % mMaxGeneVlaue + 0;
        for (size_t i = 0; i < parent1.chromosome.size(); ++i)
        {
            if (RandomFloat() < MutationRate)
            {
                offspring.chromosome[i] = std::rand() % mMaxGeneVlaue + 0;
            }
        }
    }
}

void GameState::Initialize()
{
    auto graphicsSystem = GraphicsSystem::Get();
    graphicsSystem->SetClearColor(Colors::Gray);
    
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
        auto& best = mGA.GetBestGenome();
        if (best.fitness < (int)kTarget.size())
        {
            mGA.Advance();

            std::string bestStr;
            for (auto& gene : mGA.GetBestGenome().chromosome)
            {
                bestStr += (char)(gene + 32);
            }
            mAppLog.AddLog("Generation %d: %s\n", mGA.GetGeneration() , bestStr.c_str());
        }
    }
}

void GameState::DebugUI()
{
    ImGui::Begin("controls", nullptr, ImGuiWindowFlags_AlwaysAutoResize);


    if (ImGui::Button("Run"))
    {
        mAppLog.AddLog("Initializeing new population ...\n");
        mGA.Initialize(100, (int)kTarget.size(), kValidGeneValue,0.45f, 0.1f, GeneratePopupation , ComputeStringScore, ComputeCrossOverFunc, ComputeMutationRateFunc);
        mInitialized = true;
    }
    ImGui::End();

    mAppLog.Draw("console");
}



