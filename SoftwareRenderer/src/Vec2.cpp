#include "pch.h"

#include "Vec2.h"
#include "Vec3.h"

Vec2::Vec2()
	: x(0.0f), y(0.0f)
{
}

Vec2::Vec2(float x, float y)
	: x(x), y(y)
{
}

Vec2::Vec2(const Vec3& v)
	: x(v.x), y(v.y)
{
}

Vec2 Vec2::operator-() const
{
	return { -x, -y };
}

Vec2& Vec2::operator+= (const Vec2& rhs)
{
	x += rhs.x;
	y += rhs.y;
	return *this;
}

Vec2& Vec2::operator*= (float m)
{
	x *= m;
	y *= m;
	return *this;
}

Vec2& Vec2::operator/= (float d)
{
	return *this *= 1 / d;
}

Vec2& Vec2::operator=(const Vec2& rhs)
{
	x = rhs.x;
	y = rhs.y;
	return *this;
}

bool Vec2::operator== (const Vec2& rhs) const
{
	return x == rhs.x && y == rhs.y;
}

bool Vec2::operator!= (const Vec2& rhs) const
{
	return x != rhs.x && y != rhs.y;
}

float Vec2::Length() const
{
	return std::sqrt(LengthSquared());
}

float Vec2::LengthSquared() const
{
	return (x * x + y * y);
}

Vec2 Vec2::GetNormalized() const
{
	Vec2 n = *this;
	float length = Length();
	if (length != 0.0f)
	{
		n.x /= length;
		n.y /= length;
	}
	return n;
}

std::ostream& operator<<(std::ostream& out, const Vec2& v)
{
	return out << "[" << v.x << ", " << v.y << "]" << std::endl;
}

Vec2 operator+(const Vec2& u, const Vec2& v)
{
	return { u.x + v.x, u.y + v.y };
}

Vec2 operator-(const Vec2& u, const Vec2& v)
{
	return { u.x - v.x, u.y - v.y };
}

Vec2 operator*(float t, const Vec2& v)
{
	return { t * v.x, t * v.y };
}

Vec2 operator*(const Vec2& v, float t)
{
	return t * v;
}

Vec2 operator/(const Vec2& v, float t)
{
	return (1 / t) * v;
}

float Dot(const Vec2& u, const Vec2& v)
{
	return u.x * v.x + u.y * v.y;
}

float Cross(const Vec2& u, const Vec2& v)
{
	return u.x * v.y - u.y * v.x;
}