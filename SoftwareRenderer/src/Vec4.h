#pragma once

#include <cmath>

#include "Vec3.h"

class Vec4
{
public:
	float x, y, z, w;

public:
	Vec4()
		: x(0.0f), y(0.0f), z(0.0f), w(1.0f)
	{
	}

	Vec4(float x, float y, float z, float w = 1.0f)
		: x(x), y(y), z(z), w(w)
	{
	}

	Vec4(const Vec3& v)
	{
		Vec4(v.x, v.y, v.z, 1.0f);
	}

	Vec4& operator+= (const Vec3& rhs)
	{
		x += rhs.x;
		y += rhs.y;
		z += rhs.z;
		return *this;
	}

	Vec4& operator+= (const Vec4& rhs)
	{
		x += rhs.x;
		y += rhs.y;
		z += rhs.z;
		w += rhs.w;
		return *this;
	}

	Vec4& operator*= (float m)
	{
		x *= m;
		y *= m;
		z *= m;
		w *= m;
		return *this;
	}

	Vec4& operator*= (const Vec4& rhs)
	{
		x *= rhs.x;
		y *= rhs.y;
		z *= rhs.z;
		w *= rhs.w;
		return *this;
	}

	Vec4& operator/= (float d)
	{
		return *this *= 1 / d;
	}

	Vec4& operator=(const Vec4& rhs)
	{
		x = rhs.x;
		y = rhs.y;
		z = rhs.z;
		w = rhs.w;
		return *this;
	}

	bool operator== (const Vec4& rhs) const
	{
		return x == rhs.x && y == rhs.y && z == rhs.z && w == rhs.w;
	}

	bool operator!= (const Vec4& rhs) const
	{
		return x != rhs.x && y != rhs.y && z != rhs.z && w != rhs.w;
	}
};

inline Vec4 operator+(const Vec4& u, const Vec4& v)
{
	return Vec4(u.x + v.x, u.y + v.y, u.z + v.z, u.w + v.w);
}

inline Vec4 operator-(const Vec4& u, const Vec4& v)
{
	return Vec4(u.x - v.x, u.y - v.y, u.z - v.z, u.w - v.w);
}