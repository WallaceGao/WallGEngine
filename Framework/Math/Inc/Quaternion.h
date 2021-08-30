#pragma once

namespace WallG::Math
{
    struct Matrix4;

    struct Quaternion
    {
        float w, x, y, z;

        constexpr Quaternion() noexcept : w(1.0f), x(0.0f), y(0.0f), z(0.0f) {}
        constexpr Quaternion(float w, float x, float y, float z) noexcept :w(w), x(x), y(y), z(z) {}

        const static Quaternion Identity;

        static Quaternion RotationAxis(const Vector3& axis, float rad);
        static Quaternion RotationEuler(const Vector3& eulerAngles);
        static Quaternion RotationMatrix(const Matrix4& m);
        static Quaternion RotationLook(Vector3 direction, Vector3 up = Vector3::YAxis);
        static Quaternion RotationFromTo(Vector3 from, Vector3 to);

        constexpr Quaternion operator-() const { return Quaternion(-w, -x,-y,-z); }
        constexpr Quaternion operator+(const Quaternion& q) const { return Quaternion(w + q.w, x + q.x, y + q.y, z + q.z); }
        constexpr Quaternion operator-(const Quaternion& q) const { return Quaternion(w - q.w, x - q.x, y - q.y, z - q.z); }
        constexpr Quaternion operator*(const Quaternion& q) const { return Quaternion(
            (w * q.w) - (x * q.x) - (y * q.y) - (z * q.z),
            (w * q.x) + (x * q.w) - (y * q.z) + (z * q.y),
            (w * q.y) + (x * q.z) + (y * q.w) - (z * q.x),
            (w * q.z) - (x * q.y) + (y * q.x) + (z * q.w)
            );
        }
        constexpr Quaternion operator*(float s) const { return Quaternion(w * s, x * s, y * s, z * s); }
        constexpr Quaternion operator/(float s) const { return Quaternion(w / s, x / s, y / s, z / s); }
    };

    // Add to Matrix4.h:
    // static Matrix4 RotationQuaternion(const Quaternion& q);

    // Add to EngineMath.h:
    // inline float Magnitude(const Quaternion& q);
    // inline Quaternion Normalize(const Quaternion& q);
    // constexpr Quaternion Lerp(Quaternion q0, Quaternion q1, float t);
    // Quaternion Slerp(Quaternion q0, Quaternion q1, float t);
}
