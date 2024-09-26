#pragma once

#include "Mat4x4.h"
#include "Vec3.h"

struct Camera
{
	Vec3 position;
	Vec3 lookDir;
	Mat4x4 mat;
	float roll;
	float yaw;
	float pitch;
};