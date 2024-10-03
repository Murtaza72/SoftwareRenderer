#pragma once

#include <iostream>
#include <cmath>

#include "Vec3.h"

class Vec2
{
public:
	float x, y;

public:
	Vec2()
		: x(0.0f), y(0.0f)
	{
	}

	Vec2(float x, float y)
		: x(x), y(y)
	{
	}

	Vec2(Vec3 v)
		: x(v.x), y(v.y)
	{
	}

	Vec2 operator-() const
	{
		return Vec2(-x, -y);
	}

	Vec2& operator+= (const Vec2& rhs)
	{
		x += rhs.x;
		y += rhs.y;
		return *this;
	}

	Vec2& operator*= (float m)
	{
		x *= m;
		y *= m;
		return *this;
	}

	Vec2& operator/= (float d)
	{
		return *this *= 1 / d;
	}

	Vec2& operator=(const Vec2& rhs)
	{
		x = rhs.x;
		y = rhs.y;
		return *this;
	}

	bool operator== (const Vec2& rhs) const
	{
		return x == rhs.x && y == rhs.y;
	}

	bool operator!= (const Vec2& rhs) const
	{
		return x != rhs.x && y != rhs.y;
	}

	float Length() const
	{
		return std::sqrt(LengthSquared());
	}

	float LengthSquared() const
	{
		return (x * x + y * y);
	}

	Vec2& Normalize()
	{
		float length = Length();
		x /= length;
		y /= length;
		return *this;
	}
};

inline std::ostream& operator<<(std::ostream& out, const Vec2& v)
{
	return out << "[" << v.x << ", " << v.y << "]";
}

inline Vec2 operator+(const Vec2& u, const Vec2& v)
{
	return Vec2(u.x + v.x, u.y + v.y);
}

inline Vec2 operator-(const Vec2& u, const Vec2& v)
{
	return Vec2(u.x - v.x, u.y - v.y);
}

inline Vec2 operator*(float t, const Vec2& v)
{
	return Vec2(t * v.x, t * v.y);
}

inline Vec2 operator*(const Vec2& v, float t)
{
	return t * v;
}

inline Vec2 operator/(const Vec2& v, float t)
{
	return (1 / t) * v;
}

inline float Dot(const Vec2& u, const Vec2& v)
{
	return u.x * v.x + u.y * v.y;
}

inline float Cross(const Vec2& u, const Vec2& v)
{
	return u.x * v.y - u.y * v.x;
}