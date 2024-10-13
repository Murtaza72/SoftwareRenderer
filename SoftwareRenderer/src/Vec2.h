#pragma once

#include "pch.h"
#include "Vec3.h"

class Vec2
{
public:
	float x, y;

public:
	Vec2();
	Vec2(float x, float y);
	Vec2(const Vec3& v);

	Vec2 operator-() const;
	Vec2& operator+= (const Vec2& rhs);
	Vec2& operator*= (float m);
	Vec2& operator/= (float d);
	Vec2& operator=(const Vec2& rhs);
	bool operator== (const Vec2& rhs) const;
	bool operator!= (const Vec2& rhs) const;

	float Length() const;
	float LengthSquared() const;
	Vec2 GetNormalized() const;
};

std::ostream& operator<<(std::ostream& out, const Vec2& v);

Vec2 operator+(const Vec2& u, const Vec2& v);
Vec2 operator-(const Vec2& u, const Vec2& v);
Vec2 operator*(float t, const Vec2& v);
Vec2 operator*(const Vec2& v, float t);
Vec2 operator/(const Vec2& v, float t);

float Dot(const Vec2& u, const Vec2& v);
float Cross(const Vec2& u, const Vec2& v);