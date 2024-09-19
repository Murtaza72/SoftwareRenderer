#include "Renderer.h"

#include <iostream>
#include <chrono>
#include <thread>
#include <time.h>
#include <stdlib.h>
#include <vector>

#define INVERT_Y_AXIS 1

struct Vec2
{
	float x, y;
};

struct Vec3
{
	float x, y, z;
};

struct Triangle
{
	Vec3 p[3];
};

struct Mesh
{
	std::vector<Triangle> tris;
};

struct Mat4x4
{
	float mat[4][4] = { 0 };
};

void MultiplyMatrixVector(const Vec3& in, Vec3& out, const Mat4x4& mat)
{
	out.x = in.x * mat.mat[0][0] + in.y * mat.mat[1][0] + in.z * mat.mat[2][0] + mat.mat[3][0];
	out.y = in.x * mat.mat[0][1] + in.y * mat.mat[1][1] + in.z * mat.mat[2][1] + mat.mat[3][1];
	out.z = in.x * mat.mat[0][2] + in.y * mat.mat[1][2] + in.z * mat.mat[2][2] + mat.mat[3][2];
	float w = in.x * mat.mat[0][3] + in.y * mat.mat[1][3] + in.z * mat.mat[2][3] + mat.mat[3][3];

	if (w != 0.0f)
	{
		out.x /= w;
		out.y /= w;
		out.z /= w;
	}
}

Renderer::~Renderer()
{
	SDL_DestroyRenderer(m_Renderer);
	SDL_DestroyWindow(m_Window);
	SDL_Quit();
}

void Renderer::Render()
{
	JavidDemo();
}

void Renderer::JavidDemo()
{
	Mesh cubeMesh;
	cubeMesh.tris = {
		{0.0f, 0.0f, 0.0f,	0.0f, 1.0f, 0.0f,	1.0f, 1.0f, 0.0f},
		{0.0f, 0.0f, 0.0f,	1.0f, 1.0f, 0.0f,	1.0f, 0.0f, 0.0f},

		{1.0f, 0.0f, 0.0f,	1.0f, 1.0f, 0.0f,	1.0f, 1.0f, 1.0f},
		{1.0f, 0.0f, 0.0f,	1.0f, 1.0f, 1.0f,	1.0f, 0.0f, 1.0f},

		{1.0f, 0.0f, 1.0f,	1.0f, 1.0f, 1.0f,	0.0f, 1.0f, 1.0f},
		{1.0f, 0.0f, 1.0f,	0.0f, 1.0f, 1.0f,	0.0f, 0.0f, 1.0f},

		{0.0f, 0.0f, 1.0f,	0.0f, 1.0f, 1.0f,	0.0f, 1.0f, 0.0f},
		{0.0f, 0.0f, 1.0f,	0.0f, 1.0f, 0.0f,	0.0f, 0.0f, 0.0f},

		{0.0f, 1.0f, 0.0f,	0.0f, 1.0f, 1.0f,	1.0f, 1.0f, 1.0f},
		{0.0f, 1.0f, 0.0f,	1.0f, 1.0f, 1.0f,	1.0f, 1.0f, 0.0f},

		{1.0f, 0.0f, 1.0f,	0.0f, 0.0f, 1.0f,	0.0f, 0.0f, 0.0f},
		{1.0f, 0.0f, 1.0f,	0.0f, 0.0f, 0.0f,	1.0f, 0.0f, 0.0f},
	};

	float theta = 0;

	float nearPlane = 0.1f;
	float farPlane = 1000.0f;
	float fov = 90.0f;
	float aspectRatio = (float)GetWindowWidth() / GetWindowHeight();
	float fovRad = 1.0f / tanf(fov * 0.5 / 180.0f * 3.1415f);		// convert theta to radians

	Mat4x4 projectionMat;
	projectionMat.mat[0][0] = aspectRatio * fovRad;
	projectionMat.mat[1][1] = fovRad;
	projectionMat.mat[2][2] = farPlane / (farPlane - nearPlane);
	projectionMat.mat[3][2] = (-farPlane * nearPlane) / (farPlane - nearPlane);
	projectionMat.mat[2][3] = 1.0f;
	projectionMat.mat[3][3] = 0.0f;

	theta += SDL_GetTicks() / 500.0f;

	Mat4x4 rotationMatX, rotationMatY, rotationMatZ;
	rotationMatX.mat[0][0] = 1;
	rotationMatX.mat[1][1] = cosf(0.5f * theta);
	rotationMatX.mat[1][2] = sinf(0.5f * theta);
	rotationMatX.mat[2][1] = -sinf(0.5f * theta);
	rotationMatX.mat[2][2] = cosf(0.5f * theta);
	rotationMatX.mat[3][3] = 1;

	rotationMatY.mat[0][0] = cosf(0.5f * theta);
	rotationMatY.mat[0][2] = sinf(0.5f * theta);
	rotationMatY.mat[2][0] = -sinf(0.5f * theta);
	rotationMatY.mat[2][2] = cosf(0.5f * theta);
	rotationMatY.mat[1][1] = 1;
	rotationMatY.mat[3][3] = 1;

	rotationMatZ.mat[0][0] = cosf(0.5f * theta);
	rotationMatZ.mat[0][1] = sinf(0.5f * theta);
	rotationMatZ.mat[1][0] = -sinf(0.5f * theta);
	rotationMatZ.mat[1][1] = cosf(0.5f * theta);
	rotationMatZ.mat[2][2] = 1;
	rotationMatZ.mat[3][3] = 1;

	#define INVERT_Y_AXIS 0

	for (auto tri : cubeMesh.tris)
	{
		Triangle triProjected, triTranslated, triRotatedZ, triRotatedZY, triRotatedZYX;

		MultiplyMatrixVector(tri.p[0], triRotatedZ.p[0], rotationMatX);
		MultiplyMatrixVector(tri.p[1], triRotatedZ.p[1], rotationMatX);
		MultiplyMatrixVector(tri.p[2], triRotatedZ.p[2], rotationMatX);

		MultiplyMatrixVector(triRotatedZ.p[0], triRotatedZY.p[0], rotationMatY);
		MultiplyMatrixVector(triRotatedZ.p[1], triRotatedZY.p[1], rotationMatY);
		MultiplyMatrixVector(triRotatedZ.p[2], triRotatedZY.p[2], rotationMatY);

		MultiplyMatrixVector(triRotatedZY.p[0], triRotatedZYX.p[0], rotationMatZ);
		MultiplyMatrixVector(triRotatedZY.p[1], triRotatedZYX.p[1], rotationMatZ);
		MultiplyMatrixVector(triRotatedZY.p[2], triRotatedZYX.p[2], rotationMatZ);

		triTranslated = triRotatedZYX;
		triTranslated.p[0].z = triRotatedZYX.p[0].z + 3.0f;
		triTranslated.p[1].z = triRotatedZYX.p[1].z + 3.0f;
		triTranslated.p[2].z = triRotatedZYX.p[2].z + 3.0f;

		#if 0
		// Projection
		triTranslated.p[0].x /= triTranslated.p[0].z;
		triTranslated.p[0].y /= triTranslated.p[0].z;
		triTranslated.p[1].x /= triTranslated.p[1].z;
		triTranslated.p[1].y /= triTranslated.p[1].z;
		triTranslated.p[2].x /= triTranslated.p[2].z;
		triTranslated.p[2].y /= triTranslated.p[2].z;
		triProjected.p[0] = triTranslated.p[0];
		triProjected.p[1] = triTranslated.p[1];
		triProjected.p[2] = triTranslated.p[2];

		#else
		MultiplyMatrixVector(triTranslated.p[0], triProjected.p[0], projectionMat);
		MultiplyMatrixVector(triTranslated.p[1], triProjected.p[1], projectionMat);
		MultiplyMatrixVector(triTranslated.p[2], triProjected.p[2], projectionMat);
		#endif

		triProjected.p[0].x += 1.0f; triProjected.p[0].y += 1.0f;
		triProjected.p[1].x += 1.0f; triProjected.p[1].y += 1.0f;
		triProjected.p[2].x += 1.0f; triProjected.p[2].y += 1.0f;

		triProjected.p[0].x *= 0.5f * (float)GetWindowWidth();
		triProjected.p[0].y *= 0.5f * (float)GetWindowHeight();
		triProjected.p[1].x *= 0.5f * (float)GetWindowWidth();
		triProjected.p[1].y *= 0.5f * (float)GetWindowHeight();
		triProjected.p[2].x *= 0.5f * (float)GetWindowWidth();
		triProjected.p[2].y *= 0.5f * (float)GetWindowHeight();

		DrawTriangle(triProjected.p[0].x, triProjected.p[0].y,
			triProjected.p[1].x, triProjected.p[1].y,
			triProjected.p[2].x, triProjected.p[2].y
		);
	}
}

void Renderer::DrawPixel(float x, float y)
{
	SDL_RenderDrawPointF(m_Renderer, x, y);
}

void Renderer::ClearColor(int r, int g, int b)
{
	SDL_SetRenderDrawColor(m_Renderer, r, g, b, 0);
	SDL_RenderClear(m_Renderer);
}

void Renderer::DrawColor(int r, int g, int b)
{
	SDL_SetRenderDrawColor(m_Renderer, r, g, b, 255);
}

void Renderer::DrawTriangle(float p1X, float p1Y, float p2X, float p2Y, float p3X, float p3Y)
{
	DrawLine(p1X, p1Y, p2X, p2Y);
	DrawLine(p2X, p2Y, p3X, p3Y);
	DrawLine(p3X, p3Y, p1X, p1Y);
}

void Renderer::FillTriangle(Vec2 p1, Vec2 p2, Vec2 p3)
{


}

void Renderer::DrawLine(float x1, float y1, float x2, float y2)
{
	if (abs(x2 - x1) > abs(y2 - y1))
		BresenhamHorizontal(x1, y1, x2, y2);
	else
		BresenhamVertical(x1, y1, x2, y2);

	//SDL_RenderPresent(m_Renderer);
	//std::this_thread::sleep_for(std::chrono::milliseconds(100));
}

void Renderer::DrawRectangle(float startX, float startY, float endX, float endY)
{
	DrawLine(startX, startY, startX, endY);
	DrawLine(startX, endY, endX, endY);
	DrawLine(endX, endY, endX, startY);
	DrawLine(endX, startY, startX, startY);
}

void Renderer::BresenhamNaive(int x1, int y1, int x2, int y2)
{
	// Works for slope < 1 only

	// invert y-axis
	y1 = GetWindowHeight() - y1;
	y2 = GetWindowHeight() - y2;

	int x = x1;
	int y = y1;
	int dx = x2 - x1;
	int dy = y2 - y1;
	float m = (float)dy / dx;
	int p = 2 * dy - dx;

	if (m > 1.0)
	{
		std::cout << "Slope greater than 1" << std::endl;
		return;
	}

	while (x <= x2)
	{
		DrawPixel(x++, y);

		if (p < 0)
		{
			p += 2 * dy;
		}
		else
		{
			p += 2 * dy - 2 * dx;
			y++;
		}
	}
}

void Renderer::BresenhamHorizontal(float x1, float y1, float x2, float y2)
{
	// invert y-axis
	#if INVERT_Y_AXIS
	y1 = GetWindowHeight() - y1;
	y2 = GetWindowHeight() - y2;
	#endif

	// Swap em
	if (x1 > x2)
	{
		float temp = x1;
		x1 = x2;
		x2 = temp;

		temp = y1;
		y1 = y2;
		y2 = temp;
	}

	float x = x1;
	float y = y1;
	float dx = x2 - x1;
	float dy = y2 - y1;

	float p = 2 * dy - dx;

	float dir = dy < 0 ? -1 : 1;
	dy *= dir;

	while (x <= x2)
	{
		DrawPixel(x++, y);

		if (p < 0)
		{
			p += 2 * dy;
		}
		else
		{
			p += 2 * (dy - dx);
			y += dir;
		}
	}
}

void Renderer::BresenhamVertical(float x1, float y1, float x2, float y2)
{
	// invert y-axis
	#if INVERT_Y_AXIS
	y1 = GetWindowHeight() - y1;
	y2 = GetWindowHeight() - y2;
	#endif

	// Swap em
	if (y1 > y2)
	{
		float temp = x1;
		x1 = x2;
		x2 = temp;

		temp = y1;
		y1 = y2;
		y2 = temp;
	}

	float x = x1;
	float y = y1;
	float dx = x2 - x1;
	float dy = y2 - y1;

	float p = 2 * dx - dy;

	float dir = dx < 0 ? -1 : 1;
	dx *= dir;

	while (y <= y2)
	{
		DrawPixel(x, y++);

		if (p < 0)
		{
			p += 2 * dx;
		}
		else
		{
			p += 2 * (dx - dy);
			x += dir;
		}
	}
}

int Renderer::GetWindowWidth()
{
	int width;
	SDL_GetWindowSize(m_Window, &width, nullptr);
	return width;
}

int Renderer::GetWindowHeight()
{
	int height;
	SDL_GetWindowSize(m_Window, nullptr, &height);
	return height;
}

void Renderer::DemoCube()
{
	srand(time(NULL));

	{
		ClearColor(0, 0, 0);

		int r = rand();
		int g = rand();
		int b = rand();
		DrawColor(r, g, b);
		DrawRectangle(100, 100, 500, 500);

		r = rand();
		g = rand();
		b = rand();
		DrawColor(r, g, b);
		DrawRectangle(200, 200, 600, 600);

		DrawLine(100, 500, 200, 600);
		DrawLine(500, 500, 600, 600);
		DrawLine(500, 100, 600, 200);
		DrawLine(100, 100, 200, 200);
	}
}