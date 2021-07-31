#pragma once

namespace WallG::Math
{
	struct Matrix3
	{
		float _11, _12, _13;
		float _21, _22, _23;
		float _31, _32, _33;

		constexpr Matrix3(
			float _11, float _12, float _13,
			float _21, float _22, float _23,
			float _31, float _32, float _33)
			:_11(_11), _12(_12), _13(_13),
			 _21(_21), _22(_22), _23(_23),
			 _31(_31), _32(_32), _33(_33)
		{
		}

		constexpr Matrix3()
			:_11(1.0f), _12(0.0f), _13(0.0f),
			 _21(0.0f), _22(1.0f), _23(0.0f),
			 _31(0.0f), _32(0.0f), _33(1.0f)
		{
		}
		

	};



}
