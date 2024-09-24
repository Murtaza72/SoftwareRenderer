#pragma once

#include "Vec3.h"

class Color
{
public:
	unsigned char r, g, b;

public:
	Color(unsigned char r, unsigned char g, unsigned char b)
		: r(r), g(g), b(b)
	{
	}

	Color(Vec3 color)
		: Color((unsigned char)color.x, (unsigned char)color.y, (unsigned char)color.z)
	{
	}
};

inline Color operator* (Color color, float m)
{
	color.r *= m;
	color.g *= m;
	color.b *= m;

	return color;
}

namespace Colors {
	static Color White = Color(255u, 255u, 255u);
	static Color Black = Color(0u, 0u, 0u);
	static Color Grey = Color(127u, 127u, 127u);
	static Color Red = Color(255u, 0u, 0u);
	static Color Green = Color(0u, 255u, 0u);
	static Color Blue = Color(0u, 0u, 255u);
	static Color Magenta = Color(255u, 0u, 255u);
	static Color Cyan = Color(0u, 255u, 255u);
	static Color Yellow = Color(255u, 255u, 0u);
}
