#pragma once

class Vec4;

class Vec3
{
public:
	float x, y, z;

public:
	Vec3();
	Vec3(float x, float y, float z);
	Vec3(const Vec4& v);

	Vec3 operator-() const;
	Vec3& operator+= (const Vec3& rhs);
	Vec3& operator*= (float m);
	Vec3& operator*= (const Vec3& rhs);
	Vec3& operator/= (float d);
	Vec3& operator=(const Vec3& rhs);
	bool operator== (const Vec3& rhs) const;
	bool operator!= (const Vec3& rhs) const;

	float Length() const;
	float LengthSquared() const;
	Vec3 GetNormalized() const;
};

std::ostream& operator<<(std::ostream& out, const Vec3& v);
Vec3 operator+(const Vec3& u, const Vec3& v);
Vec3 operator-(const Vec3& u, const Vec3& v);
Vec3& operator-=(Vec3& u, const Vec3& v);
Vec3 operator*(float t, const Vec3& v);
Vec3 operator*(const Vec3& v, float t);
Vec3 operator/(const Vec3& v, float t);

float Dot(const Vec3& u, const Vec3& v);
Vec3 Cross(const Vec3& u, const Vec3& v);