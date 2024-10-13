#pragma once

#include "pch.h"

class Vec3
{
public:
	float x, y, z;

public:
	Vec3()
		: x(0.0f), y(0.0f), z(0.0f)
	{
	}

	Vec3(float x, float y, float z)
		: x(x), y(y), z(z)
	{
	}

	Vec3 operator-() const
	{
		return Vec3(-x, -y, -z);
	}

	Vec3& operator+= (const Vec3& rhs)
	{
		x += rhs.x;
		y += rhs.y;
		z += rhs.z;
		return *this;
	}

	Vec3& operator*= (float m)
	{
		x *= m;
		y *= m;
		z *= m;
		return *this;
	}

	Vec3& operator*= (const Vec3& rhs)
	{
		x *= rhs.x;
		y *= rhs.y;
		z *= rhs.z;
		return *this;
	}

	Vec3& operator/= (float d)
	{
		return *this *= 1 / d;
	}

	Vec3& operator=(const Vec3& rhs)
	{
		x = rhs.x;
		y = rhs.y;
		z = rhs.z;
		return *this;
	}

	bool operator== (const Vec3& rhs) const
	{
		return x == rhs.x && y == rhs.y && z == rhs.z;
	}

	bool operator!= (const Vec3& rhs) const
	{
		return x != rhs.x && y != rhs.y && z != rhs.z;
	}

	float Length() const
	{
		return std::sqrt(LengthSquared());
	}

	float LengthSquared() const
	{
		return (x * x + y * y + z * z);
	}

	Vec3 GetNormalized() const
	{
		Vec3 n = *this;
		float length = Length();
		if (length != 0.0f)
		{
			n.x /= length;
			n.y /= length;
			n.z /= length;
		}
		return n;
	}
};

inline std::ostream& operator<<(std::ostream& out, const Vec3& v)
{
	return out << "[" << v.x << ", " << v.y << ", " << v.z << "]" << std::endl;
}

inline Vec3 operator+(const Vec3& u, const Vec3& v)
{
	return Vec3(u.x + v.x, u.y + v.y, u.z + v.z);
}

inline Vec3 operator-(const Vec3& u, const Vec3& v)
{
	return Vec3(u.x - v.x, u.y - v.y, u.z - v.z);
}

inline Vec3& operator-=(Vec3& u, const Vec3& v)
{
	u.x = u.x - v.x;
	u.y = u.y - v.y;
	u.z = u.z - v.z;
	return u;
}

inline Vec3 operator*(float t, const Vec3& v)
{
	return Vec3(t * v.x, t * v.y, t * v.z);
}

inline Vec3 operator*(const Vec3& v, float t)
{
	return t * v;
}

inline Vec3 operator/(const Vec3& v, float t)
{
	return (1 / t) * v;
}

inline float Dot(const Vec3& u, const Vec3& v)
{
	return u.x * v.x + u.y * v.y + u.z * v.z;
}

inline Vec3 Cross(const Vec3& u, const Vec3& v)
{
	return Vec3(u.y * v.z - u.z * v.y,
		u.z * v.x - u.x * v.z,
		u.x * v.y - u.y * v.x);
}