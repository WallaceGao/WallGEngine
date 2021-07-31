#pragma once

namespace WallG::ML
{
	struct Dataset;
	
	class LinearRegression
	{
	public:
		float Fit(const Dataset& dataset);
		float Predict(float X) const;

		float b0 = 0.0f;
		float b1 = 0.0f;
		float learningRate = 0.01f;
	};
}
