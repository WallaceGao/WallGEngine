
#include "Precompiled.h"
#include "LogisticsRegression.h"
#include "Datasets.h"

using namespace WallG::ML;

float WallG::ML::LogisticsRegression::Fit(const Dataset& dataset)
{
	float error0 = 0.0f;
	float error1 = 0.0f;
	float error2 = 0.0f;
	float mse = 0.0f;

	for (size_t i = 0; i < dataset.x0.size(); i++)
	{
		float predict = Predict(dataset.x0[i], dataset.x1[i]);
		float error = predict - dataset.Y[i];
		error0 += error;
		error1 += error * dataset.x0[i];
		error2 += error * dataset.x1[i];
		mse += (error * error0);
	}
	
	const float m = (float)dataset.x0.size();
	b0 -= learningRate * error0 / m;
	b1 -= learningRate * error1 / m;
	b2 -= learningRate * error2 / m;
	mse /= m;
	return mse;
}

float WallG::ML::LogisticsRegression::Predict(float x0, float x1) const
{
	float z = b0 + (b1 * x0) + (b2 * x1);
	return 1/(1+ exp(-z));
}
