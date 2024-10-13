#include "pch.h"

#include "Vec4.h"
#include "Vec3.h"

Vec4::Vec4()
	: x(0.0f), y(0.0f), z(0.0f), w(1.0f)
{
}

Vec4::Vec4(float x, float y, float z, float w)
	: x(x), y(y), z(z), w(w)
{
}

Vec4::Vec4(const Vec3& v)
	: x(v.x), y(v.y), z(v.z), w(1.0f)
{
}

Vec4& Vec4::operator+= (const Vec4& rhs)
{
	x += rhs.x;
	y += rhs.y;
	z += rhs.z;
	w += rhs.w;
	return *this;
}

Vec4& Vec4::operator*= (float m)
{
	x *= m;
	y *= m;
	z *= m;
	w *= m;
	return *this;
}

Vec4& Vec4::operator/= (float d)
{
	return *this *= (1 / d);
}

Vec4& Vec4::operator= (const Vec4& rhs)
{
	x = rhs.x;
	y = rhs.y;
	z = rhs.z;
	w = rhs.w;
	return *this;
}

bool Vec4::operator== (const Vec4& rhs) const
{
	return x == rhs.x && y == rhs.y && z == rhs.z && w == rhs.w;
}

bool Vec4::operator!= (const Vec4& rhs) const
{
	return !(*this == rhs);
}

Vec4 operator+(const Vec4& u, const Vec4& v)
{
	return { u.x + v.x, u.y + v.y, u.z + v.z, u.w + v.w };
}

Vec4 operator-(const Vec4& u, const Vec4& v)
{
	return { u.x - v.x, u.y - v.y, u.z - v.z, u.w - v.w };
}
