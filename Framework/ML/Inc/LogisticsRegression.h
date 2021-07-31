#pragma once

namespace WallG::ML
{
	struct Dataset;

	class LogisticsRegression
	{
	public:
		float Fit(const Dataset& dataset);
		float Predict(float x0, float x1) const;

		float b0 = 0.0f;
		float b1 = 0.0f;
		float b2 = 0.0f;
		float learningRate = 0.01f;
	};
}
