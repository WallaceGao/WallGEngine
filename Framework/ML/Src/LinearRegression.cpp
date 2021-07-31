
#include "Precompiled.h"
#include "LinearRegression.h"
#include "Datasets.h"

using namespace WallG::ML;

float LinearRegression::Fit(const Dataset& dataset)
{
	// homework
	float error0 = 0.0f;
	float error1 = 0.0f;
	float mse = 0.0f;
	float predict = 0.0f;
	float m = (float)dataset.x0.size();
	for (size_t i = 0; i < dataset.x0.size(); i++)
	{
		predict = Predict(dataset.x0[i]);
		float error = predict - dataset.Y[i];
		error0 += error;
		error1 += error * dataset.x0[i];
		mse += (error * error0);
	}
	b0 -= learningRate * error0 / m;
	b1 -= learningRate * error1 / m;
	mse /= m;
	return mse;
}

float LinearRegression::Predict(float X) const
{
	return b0 + (b1 * X);
}
