#pragma once

#include "Common.h"

#include "Constants.h"

#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"
#include "Matrix3.h"
#include "Matrix4.h"
#include "Quaternion.h"
#include "Geometry.h"
#include "Ray.h"

#include "AABB.h"
#include "Sphere.h"

namespace WallG::Math
{
    //Float
    float RandomFloat();
    float RandomFloat(float min, float max);
    int RandomInt(int min, int max);

    //Vector2 Math
    constexpr float Dot(const Vector2& a, const Vector2& b)
    {
        return a.x * b.x + a.y * b.y;;
    }

    inline float MagnitudeSquared(const Vector2& v)
    {
        return v.x * v.x + v.y * v.y;
    }

    inline float Magnitude(const Vector2& v)
    {
        return sqrtf(MagnitudeSquared(v));;
    }

    inline float DistanceSquared(const Vector2& a, const Vector2& b)
    {
        return MagnitudeSquared(a - b);
    }

    inline float Distance(const Vector2& a, const Vector2& b)
    {
        return sqrtf(DistanceSquared(a, b));
    }

    constexpr Vector2 Normalize(const Vector2& v)
    {
        return v / Magnitude(v);
    }

    //Vector3 Math
    constexpr float Dot(const Vector3& a, const Vector3& b)
    {
        return a.x * b.x + a.y * b.y + a.z * b.z;;
    }


    constexpr float MagnitudeSquared(const Vector3& v)
    {
        return v.x * v.x + v.y * v.y + v.z * v.z;
    }


    inline float Magnitude(const Vector3& v)
    {
        return sqrtf(MagnitudeSquared(v));;
    }

    constexpr float DistanceSquared(const Vector3& a, const Vector3& b)
    {
        return MagnitudeSquared(a - b);
    }

    inline float Distance(const Vector3& a, const Vector3& b)
    {
        return sqrtf(DistanceSquared(a,b));
    }

    inline Vector3 Normalize(const Vector3& v)
    {
        auto mag = Magnitude(v);
        if (mag <= 0.0f)
        {
            return v;
        }
        return v / mag;
    }

    constexpr Vector3 Cross(const Vector3& a, const Vector3& b)
    {
        return Vector3((a.y * b.z) - (b.y * a.z), (a.z * b.x) - (b.z * a.x), (a.x * b.y) - (b.x * a.y));
    }

    inline Vector3 TransformNormal(Vector3 v, Matrix4 m) // assume w = 0 
    {
        return Vector3(v.x * m._11 + v.y * m._21 + v.z * m._31,
            v.x * m._12 + v.y * m._22 + v.z * m._32,
            v.x * m._13 + v.y * m._23 + v.z * m._33);
    }

    inline Vector3 TransformCoord(Vector3 v, Matrix4 m) // assume w = 1 
    {
        //w needs to be 1 after transforming
        const float invW = 1.0f / ((v.x * m._14) + (v.y * m._24) + (v.z * m._34) + (1.0f * m._44));
        return Vector3
        (
            (v.x * m._11 + v.y * m._21 + v.z * m._31 + (1.0f * m._41)) * invW, //+ m._41,
            (v.x * m._12 + v.y * m._22 + v.z * m._32 + (1.0f * m._42)) * invW, //+ m._42,
            (v.x * m._13 + v.y * m._23 + v.z * m._33 + (1.0f * m._43)) * invW  //+ m._43
        );
    }

    inline float Determinant(Matrix4 m)
    {
        float determinant = 0.0f;
        determinant += (m._11 * (m._22 * (m._33 * m._44 - (m._43 * m._34)) - m._23 * (m._32 * m._44 - (m._42 * m._34)) + m._24 * (m._32 * m._43 - (m._42 * m._33))));
        determinant -= (m._12 * (m._21 * (m._33 * m._44 - (m._43 * m._34)) - m._23 * (m._31 * m._44 - (m._41 * m._34)) + m._24 * (m._31 * m._43 - (m._41 * m._33))));
        determinant += (m._13 * (m._21 * (m._32 * m._44 - (m._42 * m._34)) - m._22 * (m._31 * m._44 - (m._41 * m._34)) + m._24 * (m._31 * m._42 - (m._41 * m._32))));
        determinant -= (m._14 * (m._21 * (m._32 * m._43 - (m._42 * m._33)) - m._22 * (m._31 * m._43 - (m._41 * m._33)) + m._23 * (m._31 * m._42 - (m._41 * m._32))));
        return determinant;
    }

    inline Matrix4 Adjoint(Matrix4 m)
    {
        return Matrix4(
            (m._22 * ((m._33 * m._44) - (m._43 * m._34)) - m._23 * ((m._32 * m._44) - (m._42 * m._34)) + m._24 * ((m._32 * m._43) - (m._42 * m._33))),
            -(m._12 * ((m._33 * m._44) - (m._43 * m._34)) - m._13 * ((m._32 * m._44) - (m._42 * m._34)) + m._14 * ((m._32 * m._43) - (m._42 * m._33))),
            (m._12 * ((m._23 * m._44) - (m._43 * m._24)) - m._13 * ((m._22 * m._44) - (m._42 * m._24)) + m._14 * ((m._22 * m._43) - (m._42 * m._23))),
            -(m._12 * ((m._23 * m._34) - (m._33 * m._24)) - m._13 * ((m._22 * m._34) - (m._32 * m._24)) + m._14 * ((m._22 * m._33) - (m._32 * m._23))),

            -(m._21 * ((m._33 * m._44) - (m._43 * m._34)) - m._31 * ((m._23 * m._44) - (m._24 * m._43)) + m._41 * ((m._23 * m._34) - (m._24 * m._33))),
            (m._11 * ((m._33 * m._44) - (m._43 * m._34)) - m._13 * ((m._31 * m._44) - (m._41 * m._34)) + m._14 * ((m._31 * m._43) - (m._41 * m._33))),
            -(m._11 * ((m._23 * m._44) - (m._43 * m._24)) - m._13 * ((m._21 * m._44) - (m._41 * m._24)) + m._14 * ((m._21 * m._43) - (m._41 * m._23))),
            (m._11 * ((m._23 * m._34) - (m._33 * m._24)) - m._13 * ((m._21 * m._34) - (m._31 * m._24)) + m._14 * ((m._21 * m._33) - (m._31 * m._23))),

            (m._21 * ((m._32 * m._44) - (m._42 * m._34)) - m._31 * ((m._22 * m._44) - (m._42 * m._24)) + m._41 * ((m._22 * m._34) - (m._32 * m._24))),
            -(m._11 * ((m._32 * m._44) - (m._42 * m._34)) - m._31 * ((m._12 * m._44) - (m._42 * m._14)) + m._41 * ((m._12 * m._34) - (m._32 * m._14))),
            (m._11 * ((m._22 * m._44) - (m._42 * m._24)) - m._12 * ((m._21 * m._44) - (m._41 * m._24)) + m._14 * ((m._21 * m._42) - (m._41 * m._22))),
            -(m._11 * ((m._22 * m._34) - (m._32 * m._24)) - m._21 * ((m._12 * m._34) - (m._32 * m._14)) + m._31 * ((m._12 * m._24) - (m._22 * m._14))),

            -(m._21 * ((m._32 * m._43) - (m._42 * m._33)) - m._31 * ((m._22 * m._43) - (m._42 * m._23)) + m._41 * ((m._22 * m._33) - (m._32 * m._23))),
            (m._11 * ((m._32 * m._43) - (m._42 * m._33)) - m._12 * ((m._31 * m._43) - (m._41 * m._33)) + m._13 * ((m._31 * m._42) - (m._41 * m._32))),
            -(m._11 * ((m._22 * m._43) - (m._42 * m._23)) - m._12 * ((m._21 * m._43) - (m._41 * m._23)) + m._13 * ((m._21 * m._42) - (m._41 * m._22))),
            (m._11 * ((m._22 * m._33) - (m._32 * m._23)) - m._12 * ((m._21 * m._33) - (m._31 * m._23)) + m._13 * ((m._21 * m._32) - (m._31 * m._22)))
        );
    }

    inline Matrix4 Transpose(Matrix4 m)
    {
        return Matrix4(m._11, m._21, m._31, m._41,
            m._12, m._22, m._32, m._42,
            m._13, m._23, m._33, m._43,
            m._14, m._24, m._34, m._44);
    }

    inline Matrix4 Inverse(Matrix4 m)
    {
        const float determinant = Determinant(m);
        const float inverseDeterminant = 1.0f / determinant;
        return Adjoint(m) * inverseDeterminant;
    }

    template <class T>
    constexpr T Min(T a, T b)
    {
        return (a < b) ? a : b;
    }

    template <class T>
    constexpr T Max(T a, T b)
    {
        return (a > b) ? a : b;
    }

    template <class T>
    constexpr T Clamp(T value, T min, T max)
    {
        return Max(min, Min(max, value));
    }

    template <class T>
    constexpr T Lerp(T a, T b, float t) //Æ½¾ùÖµ
    {
        return a + (b - a) * t;
    }

    constexpr float Abs(float value)
    {
        return value >= 0.0f ? value : -value;
    }

    constexpr float Sqr(float value)
    {
        return value * value;
    }

    constexpr Vector3 GetTranslation(const Matrix4& m)
    {
        return { m._41, m._42, m._43 };
    }
    //Quternion

    inline float Magnitude(const Quaternion& q)
    {
        return sqrtf((q.w * q.w) + (q.x * q.x) + (q.y * q.y) + (q.z * q.z));
    }

    inline Quaternion Normalize(const Quaternion& q)
    {
        return q / Magnitude(q);
    }

    constexpr Quaternion Lerp(Quaternion q0, Quaternion q1, float t)
    {
        return q0 * (1.0f - t) + (q1 * t);
    }

    Quaternion Slerp(Quaternion q0, Quaternion q1, float t);

    //Ray 
    bool IntersectSphere(const Ray& ray, const Sphere& sphere, float& distance);
    
    bool IntersectAABB(const Ray& ray, const AABB& aabb, float& distEntry, float& distExit);

}

