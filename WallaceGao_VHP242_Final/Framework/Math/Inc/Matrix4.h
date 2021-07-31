#pragma once
#include <cmath>

namespace WallG::Math
{
	struct Matrix4
	{
		float _11, _12, _13, _14;
		float _21, _22, _23, _24;
		float _31, _32, _33, _34;
		float _41, _42, _43, _44;

		constexpr Matrix4(float _11, float _12, float _13, float _14,
			float _21, float _22, float _23, float _24,
			float _31, float _32, float _33, float _34,
			float _41, float _42, float _43, float _44)
			:_11(_11), _12(_12), _13(_13), _14(_14),
			_21(_21), _22(_22), _23(_23), _24(_24),
			_31(_31), _32(_32), _33(_33), _34(_34),
			_41(_41), _42(_42), _43(_43), _44(_44)
		{
		}

		constexpr Matrix4()
			:_11(1.0f), _12(0.0f), _13(0.0f), _14(0.0f),
			_21(0.0f), _22(1.0f), _23(0.0f), _24(0.0f),
			_31(0.0f), _32(0.0f), _33(1.0f), _34(0.0f),
			_41(0.0f), _42(0.0f), _43(0.0f), _44(1.0f)
		{
		}

		// 1 0 0 0
		const static Matrix4 Indentity;


		constexpr Matrix4 operator-() const 
		{
			return {
			-_11, -_12, -_13, -_14,
			-_21, -_22, -_23, -_24,
			-_31, -_32, -_33, -_34,
			-_41, -_42, -_43, -_44
			};
		}


		constexpr Matrix4 operator+(const Matrix4& rhs) const 
		{
			return {
				_11 + rhs._11, _12 + rhs._12, _13 + rhs._13, _14 + rhs._14,
				_21 + rhs._21, _22 + rhs._22, _23 + rhs._23, _24 + rhs._24,
				_31 + rhs._31, _32 + rhs._32, _33 + rhs._33, _34 + rhs._34,
				_41 + rhs._41, _42 + rhs._42, _43 + rhs._43, _44 + rhs._44
			};
		}

		constexpr Matrix4 operator-(const Matrix4& rhs) const 
		{
			return {
				_11 - rhs._11, _12 - rhs._12, _13 - rhs._13, _14 - rhs._14,
				_21 - rhs._21, _22 - rhs._22, _23 - rhs._23, _24 - rhs._24,
				_31 - rhs._31, _32 - rhs._32, _33 - rhs._33, _34 - rhs._34,
				_41 - rhs._41, _42 - rhs._42, _43 - rhs._43, _44 - rhs._44
			};
		}

		constexpr Matrix4 operator*(float rhs) const 
		{
			return {
				_11 * rhs, _12 * rhs, _13 * rhs, _14 * rhs,
				_21 * rhs, _22 * rhs, _23 * rhs, _24 * rhs,
				_31 * rhs, _32 * rhs, _33 * rhs, _34 * rhs,
				_41 * rhs, _42 * rhs, _43 * rhs, _44 * rhs
			};
		}

		constexpr Matrix4 operator/(float rhs) const 
		{
			return {
				_11 / rhs, _12 / rhs, _13 / rhs, _14 / rhs,
				_21 / rhs, _22 / rhs, _23 / rhs, _24 / rhs,
				_31 / rhs, _32 / rhs, _33 / rhs, _34 / rhs,
				_41 / rhs, _42 / rhs, _43 / rhs, _44 / rhs
			};
		}

		constexpr Matrix4 operator*(const Matrix4& rhs) const 
		{
			return {
			(_11 * rhs._11) + (_12 * rhs._21) + (_13 * rhs._31) + (_14 * rhs._41),
			(_11 * rhs._12) + (_12 * rhs._22) + (_13 * rhs._32) + (_14 * rhs._42),
			(_11 * rhs._13) + (_12 * rhs._23) + (_13 * rhs._33) + (_14 * rhs._43),
			(_11 * rhs._14) + (_12 * rhs._24) + (_13 * rhs._34) + (_14 * rhs._44),

			(_21 * rhs._11) + (_22 * rhs._21) + (_23 * rhs._31) + (_24 * rhs._41),
			(_21 * rhs._12) + (_22 * rhs._22) + (_23 * rhs._32) + (_24 * rhs._42),
			(_21 * rhs._13) + (_22 * rhs._23) + (_23 * rhs._33) + (_24 * rhs._43),
			(_21 * rhs._14) + (_22 * rhs._24) + (_23 * rhs._34) + (_24 * rhs._44),

			(_31 * rhs._11) + (_32 * rhs._21) + (_33 * rhs._31) + (_34 * rhs._41),
			(_31 * rhs._12) + (_32 * rhs._22) + (_33 * rhs._32) + (_34 * rhs._42),
			(_31 * rhs._13) + (_32 * rhs._23) + (_33 * rhs._33) + (_34 * rhs._43),
			(_31 * rhs._14) + (_32 * rhs._24) + (_33 * rhs._34) + (_34 * rhs._44),

			(_41 * rhs._11) + (_42 * rhs._21) + (_43 * rhs._31) + (_44 * rhs._41),
			(_41 * rhs._12) + (_42 * rhs._22) + (_43 * rhs._32) + (_44 * rhs._42),
			(_41 * rhs._13) + (_42 * rhs._23) + (_43 * rhs._33) + (_44 * rhs._43),
			(_41 * rhs._14) + (_42 * rhs._24) + (_43 * rhs._34) + (_44 * rhs._44)
			};
		};


		constexpr Matrix4& operator+=(const Matrix4& rhs)
		{
			_11 += rhs._11; _12 += rhs._12; _13 += rhs._13; _14 += rhs._14;
			_21 += rhs._21; _22 += rhs._22; _23 += rhs._23; _24 += rhs._24;
			_31 += rhs._31; _32 += rhs._32; _33 += rhs._33; _34 += rhs._34;
			_41 += rhs._41; _42 += rhs._42; _43 += rhs._43; _44 += rhs._44;
			return *this;
		}

		static Matrix4 RotationX(float radian)
		{
			Matrix4 temp;
			temp._22 = cosf(radian);
			temp._23 = sinf(radian);
			temp._32 = -sinf(radian);
			temp._33 = cosf(radian);
			return temp;
		}
		static Matrix4 RotationY(float rad)
		{
			Matrix4 temp;
			temp._11 = cosf(rad);
			temp._13 = -sinf(rad);
			temp._31 = sinf(rad);
			temp._33 = cosf(rad);
			return temp;
		}

		static Matrix4 RotationZ(float rad)
		{
			Matrix4 temp;
			temp._11 = cosf(rad);
			temp._12 = sinf(rad);
			temp._21 = -sinf(rad);
			temp._22 = cosf(rad);
			return temp;
		}

		static Matrix4 Scaling(Vector3 s)
		{
			Matrix4 temp;
			temp._11 = s.x;
			temp._22 = s.y;
			temp._33 = s.z;
			return temp;
		}

		static Matrix4 Scaling(float sx, float sy, float sz)
		{
			Matrix4 temp;
			temp._11 = sx;
			temp._22 = sy;
			temp._33 = sz;
			return temp;
		}

		static Matrix4 Translation(Vector3 d)
		{
			Matrix4 temp;
			temp._41 = d.x;
			temp._42 = d.y;
			temp._43 = d.z;
			return temp;
		}

		static Matrix4 Translation(float dx, float dy, float dz)
		{
			Matrix4 temp;
			temp._41 = dx;
			temp._42 = dy;
			temp._43 = dz;
			return temp;
		}

		static Matrix4 RotationAxis(const Vector3& axis, float radian);
	};
};

