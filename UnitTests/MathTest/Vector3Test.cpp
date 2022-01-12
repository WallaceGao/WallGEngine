#include "pch.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace WallG::Math;

namespace MathTest
{
	TEST_CLASS(Vector3Test)
	{
	public:
		
		TEST_METHOD(TestMethodZero)
		{
			Vector3 v;
			Assert::AreEqual(v.x, 0.0f, 0.0001f);
			Assert::AreEqual(v.y, 0.0f, 0.0001f);
			Assert::AreEqual(v.z, 0.0f, 0.0001f);
		}

		TEST_METHOD(TestMethodOne)
		{
			Vector3 v;
			v.One;
			Assert::AreEqual(v.x, 1.0f, 1.0001f);
			Assert::AreEqual(v.y, 1.0f, 1.0001f);
			Assert::AreEqual(v.z, 1.0f, 1.0001f);
		}

		TEST_METHOD(TestMethodXAxis)
		{
			Vector3 v;
			v.XAxis;
			Assert::AreEqual(v.x, 1.0f, 1.0001f);
			Assert::AreEqual(v.y, 0.0f, 0.0001f);
			Assert::AreEqual(v.z, 0.0f, 0.0001f);
		}

		TEST_METHOD(TestMethodYAxis)
		{
			Vector3 v;
			v.YAxis;
			Assert::AreEqual(v.x, 1.0f, 1.0001f);
			Assert::AreEqual(v.y, 1.0f, 1.0001f);
			Assert::AreEqual(v.z, 1.0f, 1.0001f);
		}

		TEST_METHOD(TestMethodZAxis)
		{
			Vector3 v;
			v.ZAxis;
			Assert::AreEqual(v.x, 1.0f, 1.0001f);
			Assert::AreEqual(v.y, 1.0f, 1.0001f);
			Assert::AreEqual(v.z, 1.0f, 1.0001f);
		}
		// + - * /

		TEST_METHOD(TestMethodOperatorPuls2)
		{
			Vector3 v1 = {1080,599,999};
			Vector3 v2 = {20,1,1};
			Vector3 sum;

			sum = v1 + v2;
			Assert::AreEqual(sum.x, 1100.0f, 1100.0001f);
			Assert::AreEqual(sum.y, 600.0f, 600.0001f);
			Assert::AreEqual(sum.z, 100.0f, 1000.0001f);
		}

		TEST_METHOD(TestMethodOperatorMinus2)
		{
			Vector3 v1 = { 74, 63, -55 };
			Vector3 v2 = { -20, 24, 23 };
			Vector3 sum;

			sum = v1 + v2;
			Assert::AreEqual(sum.x, 54.0f, 54.0001f);
			Assert::AreEqual(sum.y, 87.0f, 87.0001f);
			Assert::AreEqual(sum.z, -32.0f, -32.0001f);
		}

		TEST_METHOD(TestMethodOperatorTimes)
		{
			Vector3 v1 = { 20,-20, 20 };
			float rhs = 5;
			Vector3 sum;

			sum = v1 * rhs;
			Assert::AreEqual(sum.x, 100.0f, 100.0001f);
			Assert::AreEqual(sum.y, -100.0f, -100.0001f);
			Assert::AreEqual(sum.z, 100.0f, 100.0001f);
		}

		TEST_METHOD(TestMethodOperatorDivide)
		{
			Vector3 v1 = { -100, 100, -100 };
			float rhs = 5;
			Vector3 sum;

			sum = v1 / rhs;
			Assert::AreEqual(sum.x, -40.0f, -40.0001f);
			Assert::AreEqual(sum.y, 40.0f, 40.0001f);
			Assert::AreEqual(sum.z, -40.0f, -40.0001f);
		}

		// += -= *= /=

		TEST_METHOD(TestMethodPulsEqual2)
		{
			Vector3 v1 = { 1080,599,999 };
			Vector3 v2 = { 20,1,1 };

			v1 += v2;
			Assert::AreEqual(v1.x, 1100.0f, 1100.0001f);
			Assert::AreEqual(v1.y, 600.0f, 600.0001f);
			Assert::AreEqual(v1.z, 100.0f, 1000.0001f);
		}

		TEST_METHOD(TestMethodMinusEqual2)
		{
			Vector3 v1 = { 74, 63, -55 };
			Vector3 v2 = { -20, 24, 23 };

			v1 -= v2;
			Assert::AreEqual(v1.x, 94.0f, 94.0001f);
			Assert::AreEqual(v1.y, 39.0f, 39.0001f);
			Assert::AreEqual(v1.z, -78.0f, -78.0001f);
		}

		TEST_METHOD(TestMethodTimesEqual)
		{
			Vector3 v1 = { 20,-20, 20 };
			float rhs = 5;

			v1 *= rhs;
			Assert::AreEqual(v1.x, 100.0f, 100.0001f);
			Assert::AreEqual(v1.y, -100.0f, -100.0001f);
			Assert::AreEqual(v1.z, 100.0f, 100.0001f);
		}

		TEST_METHOD(TestMethodDivideEqual)
		{
			Vector3 v1 = { -100, 100, -100 };
			float rhs = 5;

			v1 /= rhs;
			Assert::AreEqual(v1.x, -40.0f, -40.0001f);
			Assert::AreEqual(v1.y, 40.0f, 40.0001f);
			Assert::AreEqual(v1.z, -40.0f, -40.0001f);
		}

		TEST_METHOD(TestDot)
		{
			float v = Dot({ 1.0f, 0.5f, 0.7f }, { 0.5f, 1.0f, 0.7f });
			Assert::AreEqual(v, 1.490f, 0.0001f);

		}

		TEST_METHOD(TestMagnitudeSquared)
		{
			float v = MagnitudeSquared({ 1.0f,2.0f,3.0f });
			Assert::AreEqual(v, 14.0f, 0.0001f);

		}

		TEST_METHOD(TestDistanceSquared)
		{
			float v = DistanceSquared({ 2.0f,3.0f,5.0f }, { 2.0f ,0.0f,9.0f});
			Assert::AreEqual(v, 25.0f, 25.0001f);
		}

		TEST_METHOD(TestMagnitude)
		{
			float v = Magnitude({ 2.0f,3.0f,4.0f });
			Assert::AreEqual(v, 5.385f, 0.0011f);

		}

		TEST_METHOD(TestCross)
		{
			Vector3 v = Cross({ 1.0f, 0.0f, 0.0f }, { 0.0f, 1.0f, 0.0f });
			Assert::AreEqual(v.x, 0.0f, 0.0001f);
			Assert::AreEqual(v.y, 0.0f, 0.0001f);
			Assert::AreEqual(v.z, 1.0f, 0.0001f);
		}
		
		TEST_METHOD(TestNormalize)
		{
			Vector3 v = Normalize({ 1.0f, 2.0f, 3.0f });
			Assert::AreEqual(v.x, 0.267f, 0.2671f);
			Assert::AreEqual(v.y, 0.534f, 0.5341f);
			Assert::AreEqual(v.z, 0.801f, 0.8011f);
		}
		

	};
}
