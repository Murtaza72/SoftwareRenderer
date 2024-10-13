#include "pch.h"

#include "Vec3.h"
#include "Vec4.h"

Vec3::Vec3()
	: x(0.0f), y(0.0f), z(0.0f)
{
}

Vec3::Vec3(float x, float y, float z)
	: x(x), y(y), z(z)
{
}

Vec3::Vec3(const Vec4& v)
	: x(v.x), y(v.y), z(v.z)
{
}

Vec3 Vec3::operator-() const
{
	return { -x, -y, -z };
}

Vec3& Vec3::operator+= (const Vec3& rhs)
{
	x += rhs.x;
	y += rhs.y;
	z += rhs.z;
	return *this;
}

Vec3& Vec3::operator*= (float m)
{
	x *= m;
	y *= m;
	z *= m;
	return *this;
}

Vec3& Vec3::operator*= (const Vec3& rhs)
{
	x *= rhs.x;
	y *= rhs.y;
	z *= rhs.z;
	return *this;
}

Vec3& Vec3::operator/= (float d)
{
	return *this *= 1 / d;
}

Vec3& Vec3::operator=(const Vec3& rhs)
{
	x = rhs.x;
	y = rhs.y;
	z = rhs.z;
	return *this;
}

bool Vec3::operator== (const Vec3& rhs) const
{
	return x == rhs.x && y == rhs.y && z == rhs.z;
}

bool Vec3::operator!= (const Vec3& rhs) const
{
	return !(*this == rhs);
}

float Vec3::Length() const
{
	return std::sqrt(LengthSquared());
}

float Vec3::LengthSquared() const
{
	return (x * x + y * y + z * z);
}

Vec3 Vec3::GetNormalized() const
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

std::ostream& operator<<(std::ostream& out, const Vec3& v)
{
	return out << "[" << v.x << ", " << v.y << ", " << v.z << "]" << std::endl;
}

Vec3 operator+(const Vec3& u, const Vec3& v)
{
	return { u.x + v.x, u.y + v.y, u.z + v.z };
}

Vec3 operator-(const Vec3& u, const Vec3& v)
{
	return { u.x - v.x, u.y - v.y, u.z - v.z };
}

Vec3& operator-=(Vec3& u, const Vec3& v)
{
	u.x = u.x - v.x;
	u.y = u.y - v.y;
	u.z = u.z - v.z;
	return u;
}

Vec3 operator*(float t, const Vec3& v)
{
	return { t * v.x, t * v.y, t * v.z };
}

Vec3 operator*(const Vec3& v, float t)
{
	return t * v;
}

Vec3 operator/(const Vec3& v, float t)
{
	return (1 / t) * v;
}

float Dot(const Vec3& u, const Vec3& v)
{
	return u.x * v.x + u.y * v.y + u.z * v.z;
}

Vec3 Cross(const Vec3& u, const Vec3& v)
{
	return {
		u.y * v.z - u.z * v.y,
		u.z * v.x - u.x * v.z,
		u.x * v.y - u.y * v.x
	};
}