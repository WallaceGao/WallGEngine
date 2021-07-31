#pragma once

namespace WallG::ML
{
	class GenetiAlgorithm
	{
	public:
		struct Genome
		{
			std::vector<int> chromosome;
			float fitness = 0.0f;
		};

		using ComputeFitnessFunc = std::function<float( const Genome&)>;
		using ComputeCrossoverFunc = std::function<void( Genome&, const Genome&, const Genome&, float)>;
		using ComputeMutationRateFunc = std::function<void( Genome&, const Genome&, float, int)>;
		using GeneratePopupationFunc = std::function<void( std::vector<Genome>&, int, int)>;

		void Initialize(
			int populationSize,
			int chromoLength,
			int maxGeneValue,
			float crossoverRate,
			float mutationRate,
			GeneratePopupationFunc GeneratePopupation,
			ComputeFitnessFunc computeFitness,
			ComputeCrossoverFunc computeCrosspver,
			ComputeMutationRateFunc computMutation);

		void Advance();

		const Genome& GetBestGenome() const { return mPopulation.front(); }
		int GetGeneration() const { return mGeneration; }
	private:
		void EvaluatePopulation();
		Genome Mate(const Genome& parent1, const Genome& parent2);

		ComputeFitnessFunc mComputeFitness;
		ComputeCrossoverFunc mComputeCrossover;
		ComputeMutationRateFunc mComputeMutation;
		GeneratePopupationFunc mGeneratePopupation;

		std::vector<Genome> mPopulation;
		int mGeneration = 0;
		int mMaxGeneVlaue = 0;

		float mCrossoverRate = 0.0f;
		float mMutationRate = 0.0f;
	};
}
