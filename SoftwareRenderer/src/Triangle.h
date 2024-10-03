#pragma once

#include "Vec3.h"
#include "Color.h"

struct TexCoord
{
	float u = 0;
	float v = 0;
	float w = 1;
};

struct Triangle
{
	Vec3 p[3];
	TexCoord tc[3];
	Color color = Colors::Magenta;
};