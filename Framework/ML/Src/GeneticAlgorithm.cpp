#include "Precompiled.h"
#include "GeneticAlgorithm.h"

using namespace WallG::Math;
using namespace WallG::ML;

namespace
{

}

void GenetiAlgorithm::Initialize(int populationSize, int chromoLength, int maxGeneValue, float crossoverRate, float mutationRate, GeneratePopupationFunc GeneratePopupation, ComputeFitnessFunc computeFitness, ComputeCrossoverFunc computeCrosspver, ComputeMutationRateFunc computMutation)
{
	mComputeFitness = std::move(computeFitness);
	mComputeCrossover = std::move(computeCrosspver);
	mComputeMutation = std::move(computMutation);
	mGeneratePopupation = std::move(GeneratePopupation);

	//Reset
	mGeneration = 0;
	mMaxGeneVlaue = maxGeneValue;
	mCrossoverRate = crossoverRate;
	mMutationRate = mutationRate;

	//Creat initial population
	mPopulation.clear();
	//resize the vector and put all the space infomation 
	mPopulation.resize(populationSize);
	mGeneratePopupation( mPopulation , chromoLength, maxGeneValue);
	//for (auto& genome : mPopulation)
	//{
	//	//resuze the vector but not push anything on it.
	//	genome.chromosome.reserve(chromoLength);
	//	for (int i = 0; i < chromoLength; ++i)
	//	{
	//		genome.chromosome.push_back(std::rand() % maxGeneValue + 0);
	//	}
	//}
	EvaluatePopulation();
}

void GenetiAlgorithm::Advance()
{
	++mGeneration;

	std::vector<Genome> newPopulation;
	newPopulation.reserve(mPopulation.size());

	//perform Elistism: keep top 10%, mate the rest using top 50
	const int eliteCount = ((int)mPopulation.size() * 10) / 100;
	for (int i = 0; i < eliteCount; ++i)
	{
		newPopulation.push_back(mPopulation[i]);
	}

	const int mateCount = (int)mPopulation.size() - eliteCount;
	const int cutoff = (int)mPopulation.size() / 2;
	for (int i = 0; i < mateCount; ++i )
	{
		const auto& parent1 = mPopulation[std::rand() % cutoff + 0];
		const auto& parent2 = mPopulation[std::rand() % cutoff + 0];
		newPopulation.push_back(Mate(parent1, parent2));
	}
	mPopulation = std::move(newPopulation);
	EvaluatePopulation();
}

void GenetiAlgorithm::EvaluatePopulation()
{
	for (auto& genome : mPopulation)
	{
		genome.fitness = mComputeFitness(genome);
	}
	std::sort(mPopulation.begin(), mPopulation.end(), [](const auto& a, const auto& b)
	{
		return a.fitness > b.fitness;
	});
}

GenetiAlgorithm::Genome GenetiAlgorithm::Mate(const Genome& parent1, const Genome& parent2)
{
	//grab this string
	Genome offspring;
	offspring.chromosome.reserve(parent1.chromosome.size());


	mComputeCrossover(offspring, parent1,parent2,mCrossoverRate);
	mComputeMutation (offspring, parent1, mMutationRate, mMaxGeneVlaue);
	////Ramdom combine it
	//for (size_t i = 0; i < parent1.chromosome.size(); ++i)
	//{
	//	//parent1: asdasdasd 
	//	//parent2: jhipnpjsa
	//	//child:   asiandasa
	//	if (RandomFloat() < mMutationRate)
	//	{
	//  offspring.chromosome.push_back(std::rand()% mMaxGeneVlaue + 0);
	//	}
	//	else if (RandomFloat() < mCrossoverRate)
	//	{
	//		offspring.chromosome.push_back(parent1.chromosome[i]);
	//	}
	//	else
	//	{
	//		offspring.chromosome.push_back(parent2.chromosome[i]);
	//	}
	//}
	return offspring;
}
