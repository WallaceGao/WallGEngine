#include "Precompiled.h"
#include "EngineMath.h"

using namespace WallG::Math;

namespace
{
    std::random_device randomDevice{};
    std::mt19937 randomGenerator{randomDevice()};
}

// Vector2
const Vector2 Vector2::Zero{ 0.0f };
const Vector2 Vector2::One{ 1.0f };
const Vector2 Vector2::XAxis{ 1.0f,0.0f };
const Vector2 Vector2::YAxis{ 0.0f,1.0f};


// Vector3
const Vector3 Vector3::Zero{ 0.0f };
const Vector3 Vector3::One{ 1.0f };
const Vector3 Vector3::XAxis{ 1.0f,0.0f,0.0f };
const Vector3 Vector3::YAxis{ 0.0f,1.0f,0.0f };
const Vector3 Vector3::ZAxis{ 0.0f,0.0f,1.0f };

// Matrix4

const Matrix4 Matrix4::Indentity
{
	1.0f, 0.0f, 0.0f, 0.0f,
	0.0f, 1.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 1.0f, 0.0f,
	0.0f, 0.0f, 0.0f, 1.0f
};

Matrix4 Matrix4::RotationAxis(const Vector3& axis, float rad)
{
    const Vector3 u = Normalize(axis);
    const float x = u.x;
    const float y = u.y;
    const float z = u.z;
    const float s = sin(rad);
    const float c = cos(rad);

    return {
        c + (x * x * (1.0f - c)),
        x * y * (1.0f - c) + z * s,
        x * z * (1.0f - c) - y * s,
        0.0f,

        x * y * (1.0f - c) - z * s,
        c + (y * y * (1.0f - c)),
        y * z * (1.0f - c) + x * s,
        0.0f,

        x * z * (1.0f - c) + y * s,
        y * z * (1.0f - c) - x * s,
        c + (z * z * (1.0f - c)),
        0.0f,

        0.0f, 0.0f, 0.0f, 1.0f
    };
}

float WallG::Math::RandomFloat()
{
    return std::uniform_real_distribution<float>{0, 1.0f}(randomGenerator);
}

float WallG::Math::RandomFloat(float min, float max)
{
    return std::uniform_real_distribution<float>{min, max}(randomGenerator);
}

Matrix4 Matrix4::RotationQuaternion(const Quaternion& q)
{
	return Matrix4
	(
		1.0f - (2.0f * q.y * q.y) - (2.0f * q.z * q.z), (2.0f * q.x * q.y) + (2.0f * q.z * q.w)       , (2.0f * q.x * q.z) - (2.0f * q.y * q.w)       , 0.0f,

		(2.0f * q.x * q.y) - (2.0f * q.z * q.w)       , 1.0f - (2.0f * q.x * q.x) - (2.0f * q.z * q.z), (2.0f * q.y * q.z) + (2.0f * q.x * q.w)       , 0.0f,

		(2.0f * q.x * q.z) + (2.0f * q.y * q.w)       , (2.0f * q.y * q.z) - (2.0f * q.x * q.w)       , 1.0f - (2.0f * q.x * q.x) - (2.0f * q.y * q.y), 0.0f,

		0.0f                                          , 0.0f                                          , 0.0f                                          , 1.0f
	);
}

// Quaternion

const Quaternion Quaternion::Identity{ 1.0f, 0.0f, 0.0f, 0.0f};

Quaternion Quaternion::RotationAxis(const Vector3& axis, float rad)
{
    Quaternion value;
    const Vector3 a = Normalize(axis);
    value.w = cosf(rad/2.0f);
    value.x = a.x * sinf(rad / 2.0f);
    value.y = a.y * sinf(rad / 2.0f);
    value.z = a.z * sinf(rad / 2.0f);
    return value;
}


Quaternion Quaternion::RotationMatrix(const Matrix4& m)
{
	Matrix4 value = Transpose(m);
	float qw, qx, qy, qz;

	float tr = value._11 + value._22 + value._33;

	if (tr > 0)
	{
		float w = sqrt(tr + 1.0f) * 2.0f;
		qw = 0.25f * w;
		qx = (value._32 - value._23) / w;
		qy = (value._13 - value._31) / w;
		qz = (value._21 - value._12) / w;
	}
	else if ((value._11 > value._22) & (value._11 > value._33))
	{
		float w = sqrt(1.0f + value._11 - value._22 - value._33) * 2.0f;
		qw = (value._32 - value._23) / w;
		qx = 0.25f * w;
		qy = (value._12 + value._21) / w;
		qz = (value._13 + value._31) / w;
	}
	else if (value._22 > value._33)
	{
		float w = sqrt(1.0f + value._22 - value._11 - value._33) * 2.0f;
		qw = (value._13 - value._31) / w;
		qx = (value._12 + value._21) / w;
		qy = 0.25f * w;
		qz = (value._23 + value._32) / w;
	}
	else
	{
		float w = sqrt(1.0f + value._33 - value._11 - value._22) * 2.0f;
		qw = (value._21 - value._12) / w;
		qx = (value._13 + value._31) / w;
		qy = (value._23 + value._32) / w;
		qz = 0.25f * w;
	}
	return Normalize(Quaternion(qw, qx, qy, qz));
}

Quaternion Quaternion::RotationLook(Vector3 direction, Vector3 up)
{
	Vector3 z = Normalize(direction);
	Vector3 x = Normalize(Cross(up, z));
	Vector3 y = Normalize(Cross(z, x));
	Matrix4 mat
	{
		x.x , x.y , x.z , 0.0f,
		y.x , y.y , y.z , 0.0f,
		z.x , z.y , z.z , 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	};
	return RotationMatrix(mat);
}

Quaternion Quaternion::RotationFromTo(Vector3 from, Vector3 to)
{
	Vector3 a = Cross(from, to);
	float w = sqrtf(Magnitude(from) * Magnitude(from)* Magnitude(to) * Magnitude(to)) + Dot(from, to);
	return Normalize(Quaternion(w, a.x, a.y, a.z));
}

Quaternion WallG::Math::Slerp(Quaternion q0, Quaternion q1, float t)
{
	float dot = (q0.w * q1.w) + (q0.x * q1.x) + (q0.y * q1.y) + (q0.z * q1.z);

	if (dot < 0.0f)
	{
		dot = -dot;
		q1.w = -q1.w;
		q1.x = -q1.x;
		q1.y = -q1.y;
		q1.z = -q1.z;
	}
	else if (dot > 0.999f)
	{
		return Normalize(Lerp(q0, q1, t));
	}

	float theta = acosf(dot);
	float sintheta = sinf(theta);
	float scale0 = sinf(theta * (1.0f - t)) / sintheta;
	float scale1 = sinf(theta * t) / sintheta;

	return Quaternion
	(
		(q0.w * scale0) + (q1.w * scale1),
		(q0.x * scale0) + (q1.x * scale1),
		(q0.y * scale0) + (q1.y * scale1),
		(q0.z * scale0) + (q1.z * scale1)
	);
}
