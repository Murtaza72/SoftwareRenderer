#pragma once

#include "Vec3.h"
#include "Color.h"

struct TexCoord
{
	float u, v;
};

struct Triangle
{
	Vec3 p[3];
	Color color = Colors::Magenta;
	TexCoord texCoord[3];
};