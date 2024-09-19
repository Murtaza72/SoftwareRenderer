#pragma once

#include "Vec3.h"
#include "Triangle.h"

class Mat4x4
{
public:
	float elements[4][4];

public:
	Mat4x4 operator*(const Mat4x4& rhs) const
	{
		Mat4x4 result;
		for (int j = 0; j < 4; j++)
		{
			for (int k = 0; k < 4; k++)
			{
				float sum = 0.0f;
				for (int i = 0; i < 4; i++)
				{
					sum += elements[j][i] * rhs.elements[i][k];
				}
				result.elements[j][k] = sum;
			}
		}
		return result;
	}

	float operator() (int x, int y) const
	{
		return elements[x][y];
	}

	static Mat4x4 Translation(float x, float y, float z)
	{
		return {
			1.0f, 0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 1.0f, 0.0f,
			x,    y,    z,	  1.0f
		};
	}

	static Mat4x4 Projection(float nearPlane, float farPlane, float aspectRatio, float fov)
	{
		float fovRad = 1.0f / tanf(fov * 0.5 / 180.0f * 3.1415f); // convert theta to radians
		float z1 = farPlane / (farPlane - nearPlane);
		float z2 = (-farPlane * nearPlane) / (farPlane - nearPlane);
		return {
			aspectRatio * fovRad,	0.0f,	0.0f,	0.0f,
			0.0f,					fovRad, 0.0f,	0.0f,
			0.0f,					0.0f,	z1,		1.0f,
			0.0f,					0.0f,	z2,		0.0f
		};
	}

	static Mat4x4 RotationX(float theta)
	{
		float cosTheta = cosf(theta);
		float sinTheta = sinf(theta);
		return {
			1.0f, 0.0f,		 0.0f,		0.0f,
			0.0f, cosTheta,	 sinTheta,	0.0f,
			0.0f, -sinTheta, cosTheta,	0.0f,
			0.0f, 0.0f,		 0.0f,		1.0f
		};
	}

	static Mat4x4 RotationY(float theta)
	{
		float cosTheta = cosf(theta);
		float sinTheta = sinf(theta);
		return {
			cosTheta, 0.0f, sinTheta,	0.0f,
			0.0f,	  1.0f,	0.0f,		0.0f,
			-sinTheta, 0.0f, cosTheta,	0.0f,
			0.0f,	  0.0f,	0.0f,		1.0f
		};
	}

	static Mat4x4 RotationZ(float theta)
	{
		float cosTheta = cosf(theta);
		float sinTheta = sinf(theta);
		return {
			cosTheta,	sinTheta, 0.0f,	0.0f,
			-sinTheta,	cosTheta, 0.0f,	0.0f,
			0.0f,		0.0f,	  1.0f,	0.0f,
			0.0f,		0.0f,	  0.0f,	1.0f
		};
	}
};

inline Vec3 MultiplyMatrixVector(const Vec3& in, const Mat4x4& mat)
{
	Vec3 out;
	out.x = in.x * mat(0, 0) + in.y * mat(1, 0) + in.z * mat(2, 0) + mat(3, 0);
	out.y = in.x * mat(0, 1) + in.y * mat(1, 1) + in.z * mat(2, 1) + mat(3, 1);
	out.z = in.x * mat(0, 2) + in.y * mat(1, 2) + in.z * mat(2, 2) + mat(3, 2);
	float w = in.x * mat(0, 3) + in.y * mat(1, 3) + in.z * mat(2, 3) + mat(3, 3);

	if (w != 0.0f)
	{
		out /= w;
	}

	return out;
}

inline Triangle TransformTriangle(const Triangle& in, const Mat4x4& mat)
{
	Triangle out;
	out.p[0] = MultiplyMatrixVector(in.p[0], mat);
	out.p[1] = MultiplyMatrixVector(in.p[1], mat);
	out.p[2] = MultiplyMatrixVector(in.p[2], mat);

	return out;
}