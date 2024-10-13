#pragma once

class Vec3;

class Vec4
{
public:
	float x, y, z, w;

	Vec4();
	Vec4(float x, float y, float z, float w = 1.0f);
	Vec4(const Vec3& v);

	Vec4& operator+= (const Vec4& rhs);
	Vec4& operator*= (float m);
	Vec4& operator/= (float d);
	Vec4& operator= (const Vec4& rhs);

	bool operator== (const Vec4& rhs) const;
	bool operator!= (const Vec4& rhs) const;
};

Vec4 operator+(const Vec4& u, const Vec4& v);
Vec4 operator-(const Vec4& u, const Vec4& v);