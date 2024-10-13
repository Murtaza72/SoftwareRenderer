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

	// TODO: create a struct for bounding box to return

	void GetBoundingBox(int& xMin, int& xMax, int& yMin, int& yMax, int width, int height) const
	{
		xMin = floor(std::min(std::min(p[0].x, p[1].x), p[2].x));
		xMax = ceil(std::max(std::max(p[0].x, p[1].x), p[2].x));
		yMin = floor(std::min(std::min(p[0].y, p[1].y), p[2].y));
		yMax = ceil(std::max(std::max(p[0].y, p[1].y), p[2].y));

		xMin = std::max(xMin, 0);
		xMax = std::min(xMax, width - 1);

		yMin = std::max(yMin, 0);
		yMax = std::min(yMax, height - 1);
	}
};

