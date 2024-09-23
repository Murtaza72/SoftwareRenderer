#include "Renderer.h"

#include <iostream>
#include <chrono>
#include <thread>
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
	Mesh cubeMesh; // clockwise winding
	cubeMesh.tris.push_back({ Vec3{0.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f}, {1.0f, 1.0f, 0.0f} });
	cubeMesh.tris.push_back({ Vec3{0.0f, 0.0f, 0.0f}, {1.0f, 1.0f, 0.0f}, {1.0f, 0.0f, 0.0f} });
	cubeMesh.tris.push_back({ Vec3{1.0f, 0.0f, 0.0f}, {1.0f, 1.0f, 0.0f}, {1.0f, 1.0f, 1.0f} });
	cubeMesh.tris.push_back({ Vec3{1.0f, 0.0f, 0.0f}, {1.0f, 1.0f, 1.0f}, {1.0f, 0.0f, 1.0f} });
	cubeMesh.tris.push_back({ Vec3{1.0f, 0.0f, 1.0f}, {1.0f, 1.0f, 1.0f}, {0.0f, 1.0f, 1.0f} });
	cubeMesh.tris.push_back({ Vec3{1.0f, 0.0f, 1.0f}, {0.0f, 1.0f, 1.0f}, {0.0f, 0.0f, 1.0f} });
	cubeMesh.tris.push_back({ Vec3{0.0f, 0.0f, 1.0f}, {0.0f, 1.0f, 1.0f}, {0.0f, 1.0f, 0.0f} });
	cubeMesh.tris.push_back({ Vec3{0.0f, 0.0f, 1.0f}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f, 0.0f} });
	cubeMesh.tris.push_back({ Vec3{0.0f, 1.0f, 0.0f}, {0.0f, 1.0f, 1.0f}, {1.0f, 1.0f, 1.0f} });
	cubeMesh.tris.push_back({ Vec3{0.0f, 1.0f, 0.0f}, {1.0f, 1.0f, 1.0f}, {1.0f, 1.0f, 0.0f} });
	cubeMesh.tris.push_back({ Vec3{1.0f, 0.0f, 1.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f, 0.0f} });
	cubeMesh.tris.push_back({ Vec3{1.0f, 0.0f, 1.0f}, {0.0f, 0.0f, 0.0f}, {1.0f, 0.0f, 0.0f} });

	float theta = 0;

	float nearPlane = 0.1f;
	float farPlane = 1000.0f;
	float fov = 90.0f;
	float aspectRatio = (float)GetWindowWidth() / GetWindowHeight();

	Mat4x4 projectionMat = Mat4x4::Projection(nearPlane, farPlane, aspectRatio, fov);

	theta += SDL_GetTicks() / 1000.0f;

	Mat4x4 rotationMatX = Mat4x4::RotationX(theta);
	Mat4x4 rotationMatY = Mat4x4::RotationY(theta);
	Mat4x4 rotationMatZ = Mat4x4::RotationZ(theta);

	#define INVERT_Y_AXIS 0

	for (auto tri : cubeMesh.tris)
	{
		tri = TransformTriangle(tri, rotationMatX);
		tri = TransformTriangle(tri, rotationMatY);
		tri = TransformTriangle(tri, rotationMatZ);

		tri.p[0].z += 3.0f;
		tri.p[1].z += 3.0f;
		tri.p[2].z += 3.0f;

		tri = TransformTriangle(tri, projectionMat);

		tri.p[0] += Vec3(1.0f, 1.0f, 0.0f);
		tri.p[1] += Vec3(1.0f, 1.0f, 0.0f);
		tri.p[2] += Vec3(1.0f, 1.0f, 0.0f);

		tri.p[0].x *= 0.5f * (float)GetWindowWidth();
		tri.p[0].y *= 0.5f * (float)GetWindowHeight();
		tri.p[1].x *= 0.5f * (float)GetWindowWidth();
		tri.p[1].y *= 0.5f * (float)GetWindowHeight();
		tri.p[2].x *= 0.5f * (float)GetWindowWidth();
		tri.p[2].y *= 0.5f * (float)GetWindowHeight();

		FillTriangle({ tri.p[0].x, tri.p[0].y },
			{ tri.p[1].x, tri.p[1].y },
			{ tri.p[2].x, tri.p[2].y }, Colors::Green
		);

		DrawTriangle(tri.p[0].x, tri.p[0].y,
			tri.p[1].x, tri.p[1].y,
			tri.p[2].x, tri.p[2].y, Colors::Blue
		);
	}
}

void Renderer::DrawPixel(float x, float y, Color color)
{
	SDL_SetRenderDrawColor(m_Renderer, color.r, color.g, color.b, 255);
	SDL_RenderDrawPointF(m_Renderer, x, y);
}

void Renderer::DrawPixel(float x, float y)
{
	DrawPixel(x, y, Colors::White);
}

void Renderer::ClearColor(Color color)
{
	ClearColor(color.r, color.g, color.b);
}

void Renderer::DrawColor(Color color)
{
	DrawColor(color.r, color.g, color.b);
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

void Renderer::DrawTriangle(Vec2 p1, Vec2 p2, Vec2 p3, Color color)
{
	DrawTriangle(p1.x, p1.y, p2.x, p2.y, p3.x, p3.y, color);
}

void Renderer::DrawTriangle(float p1X, float p1Y, float p2X, float p2Y, float p3X, float p3Y, Color color)
{
	DrawLine(p1X, p1Y, p2X, p2Y, color);
	DrawLine(p2X, p2Y, p3X, p3Y, color);
	DrawLine(p3X, p3Y, p1X, p1Y, color);
}

void Renderer::DrawLine(Vec2 p1, Vec2 p2, Color color)
{
	DrawLine(p1.x, p1.y, p2.x, p2.y, color);
}

void Renderer::DrawLine(float x1, float y1, float x2, float y2, Color color)
{
	if (abs(x2 - x1) > abs(y2 - y1))
		BresenhamHorizontal(x1, y1, x2, y2, color);
	else
		BresenhamVertical(x1, y1, x2, y2, color);

	//SDL_RenderPresent(m_Renderer);
	//std::this_thread::sleep_for(std::chrono::milliseconds(200));
}

int EdgeCross(Vec2 a, Vec2 b, Vec2 c)
{
	Vec2 ab = { b.x - a.x, b.y - a.y };
	Vec2 ac = { c.x - a.x, c.y - a.y };

	return Cross(ab, ac);
}

bool IsTopLeftEdge(Vec2 start, Vec2 end)
{
	Vec2 edge = { end.x - start.x,end.y - start.y };

	bool isTop = edge.y == 0 && edge.x > 0;
	bool isLeft = edge.y < 0;

	return isTop || isLeft;
}

void Renderer::FillTriangle(Vec2 p1, Vec2 p2, Vec2 p3, Color color)
{
	// Calculate bounding box around the tri
	int xMin = std::min(std::min(p1.x, p2.x), p3.x);
	int xMax = std::max(std::max(p1.x, p2.x), p3.x);
	int yMin = std::min(std::min(p1.y, p2.y), p3.y);
	int yMax = std::max(std::max(p1.y, p2.y), p3.y);

	// Determine if either top or left edge
	int bias1 = IsTopLeftEdge(p1, p2) ? 0 : -1;
	int bias2 = IsTopLeftEdge(p2, p3) ? 0 : -1;
	int bias3 = IsTopLeftEdge(p3, p1) ? 0 : -1;

	for (int y = yMin; y <= yMax; y++)
	{
		for (int x = xMin; x <= xMax; x++)
		{
			Vec2 p = { (float)x, (float)y };

			int w1 = EdgeCross(p1, p2, p) + bias1;
			int w2 = EdgeCross(p2, p3, p) + bias2;
			int w3 = EdgeCross(p3, p1, p) + bias3;

			bool isInside = w1 >= 0 && w2 >= 0 && w3 >= 0;

			if (isInside)
			{
				DrawPixel(x, y, color);
			}
		}
	}
}

void Renderer::DrawRectangle(Vec2 start, Vec2 end, Color color)
{
	DrawRectangle(start.x, start.y, end.x, end.y, color);
}

void Renderer::DrawRectangle(float startX, float startY, float endX, float endY, Color color)
{
	DrawTriangle(startX, startY, endX, startY, startX, endY, color);
	DrawTriangle(endX, startY, endX, endY, startX, endY, color);
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

void Renderer::BresenhamHorizontal(float x1, float y1, float x2, float y2, Color color)
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
		DrawPixel(x++, y, color);

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

void Renderer::BresenhamVertical(float x1, float y1, float x2, float y2, Color color)
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
		DrawPixel(x, y++, color);

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