#include "Renderer.h"

#include <iostream>
#include <chrono>
#include <thread>
#include <time.h>
#include <stdlib.h>
#include <vector>

#include "Triangle.h"

#define INVERT_Y_AXIS 1

struct Mesh
{
	std::vector<Triangle> tris;
};

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
	cubeMesh.tris.push_back({ Vec3{0.0f, 0.0f, 0.0f}, Vec3{0.0f, 1.0f, 0.0f}, Vec3{1.0f, 1.0f, 0.0f} });
	cubeMesh.tris.push_back({ Vec3{0.0f, 0.0f, 0.0f}, Vec3{1.0f, 1.0f, 0.0f}, Vec3{1.0f, 0.0f, 0.0f} });
	cubeMesh.tris.push_back({ Vec3{1.0f, 0.0f, 0.0f}, Vec3{1.0f, 1.0f, 0.0f}, Vec3{1.0f, 1.0f, 1.0f} });
	cubeMesh.tris.push_back({ Vec3{1.0f, 0.0f, 0.0f}, Vec3{1.0f, 1.0f, 1.0f}, Vec3{1.0f, 0.0f, 1.0f} });
	cubeMesh.tris.push_back({ Vec3{1.0f, 0.0f, 1.0f}, Vec3{1.0f, 1.0f, 1.0f}, Vec3{0.0f, 1.0f, 1.0f} });
	cubeMesh.tris.push_back({ Vec3{1.0f, 0.0f, 1.0f}, Vec3{0.0f, 1.0f, 1.0f}, Vec3{0.0f, 0.0f, 1.0f} });
	cubeMesh.tris.push_back({ Vec3{0.0f, 0.0f, 1.0f}, Vec3{0.0f, 1.0f, 1.0f}, Vec3{0.0f, 1.0f, 0.0f} });
	cubeMesh.tris.push_back({ Vec3{0.0f, 0.0f, 1.0f}, Vec3{0.0f, 1.0f, 0.0f}, Vec3{0.0f, 0.0f, 0.0f} });
	cubeMesh.tris.push_back({ Vec3{0.0f, 1.0f, 0.0f}, Vec3{0.0f, 1.0f, 1.0f}, Vec3{1.0f, 1.0f, 1.0f} });
	cubeMesh.tris.push_back({ Vec3{0.0f, 1.0f, 0.0f}, Vec3{1.0f, 1.0f, 1.0f}, Vec3{1.0f, 1.0f, 0.0f} });
	cubeMesh.tris.push_back({ Vec3{1.0f, 0.0f, 1.0f}, Vec3{0.0f, 0.0f, 1.0f}, Vec3{0.0f, 0.0f, 0.0f} });
	cubeMesh.tris.push_back({ Vec3{1.0f, 0.0f, 1.0f}, Vec3{0.0f, 0.0f, 0.0f}, Vec3{1.0f, 0.0f, 0.0f} });

	float theta = 0;

	float nearPlane = 0.1f;
	float farPlane = 1000.0f;
	float fov = 90.0f;
	float aspectRatio = (float)GetWindowWidth() / GetWindowHeight();

	Mat4x4 projectionMat = Mat4x4::Projection(nearPlane, farPlane, aspectRatio, fov);

	theta += SDL_GetTicks() / 500.0f;

	Mat4x4 rotationMatX = Mat4x4::RotationX(0.5 * theta);
	Mat4x4 rotationMatY = Mat4x4::RotationY(0.5 * theta);
	Mat4x4 rotationMatZ = Mat4x4::RotationZ(0.5 * theta);

	#define INVERT_Y_AXIS 0

	for (auto tri : cubeMesh.tris)
	{
		Triangle triProjected, triTranslated, triRotatedZ, triRotatedZY, triRotatedZYX;

		triRotatedZ = TransformTriangle(tri, rotationMatX);
		triRotatedZY = TransformTriangle(triRotatedZ, rotationMatY);
		triRotatedZYX = TransformTriangle(triRotatedZY, rotationMatZ);

		triTranslated = triRotatedZYX;
		triTranslated.p[0].z += 3.0f;
		triTranslated.p[1].z += 3.0f;
		triTranslated.p[2].z += 3.0f;

		triProjected = TransformTriangle(triTranslated, projectionMat);

		triProjected.p[0] += Vec3(1.0f, 1.0f, 0.0f);
		triProjected.p[1] += Vec3(1.0f, 1.0f, 0.0f);
		triProjected.p[2] += Vec3(1.0f, 1.0f, 0.0f);

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