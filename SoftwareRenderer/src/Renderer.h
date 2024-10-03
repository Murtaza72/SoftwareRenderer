#pragma once

#include <SDL.h>
#include <vector>

#include "Vec3.h"
#include "Vec2.h"
#include "Mat4x4.h"
#include "Color.h"
#include "Camera.h"
#include "Texture.h"

class Renderer
{
public:
	Renderer(SDL_Renderer* renderer, SDL_Window* window, Texture& tex)
		: m_Renderer(renderer), m_Window(window), m_Texture(tex)
	{
		m_DepthBuffer = new float[GetWindowWidth() * GetWindowHeight()];
	}

	~Renderer();

	void Render(Camera& camera);

	void ClearColor(Color color);
	void DrawColor(Color color);

	void ClearDepth();

	void DrawTriangle(float p1X, float p1Y, float p2X, float p2Y, float p3X, float p3Y, Color color);
	void FillTriangle(Vec2 p1, Vec2 p2, Vec2 p3, Color color);
	void FillTriangleOptimized(Vec3 p1, Vec3 p2, Vec3 p3, Color color);
	void FillTriangleTextured(Vec3 p1, Vec3 p2, Vec3 p3, TexCoord t1, TexCoord t2, TexCoord t3, Texture tex);

	void DrawLine(float x1, float y1, float x2, float y2, Color color);

	int ClipAgainstPlane(Vec3 point, Vec3 plane, Triangle& in, Triangle& outTri1, Triangle& outTri2);

	void Present();

private:
	void DrawPixel(float x, float y, Color c);
	void DrawPixel(float x, float y);

	void BresenhamNaive(int x1, int y1, int x2, int y2);
	void BresenhamHorizontal(float x1, float y1, float x2, float y2, Color color);
	void BresenhamVertical(float x1, float y1, float x2, float y2, Color color);

private:
	void JavidDemo(Camera& camera);

private:
	int GetWindowWidth();
	int GetWindowHeight();

private:
	SDL_Renderer* m_Renderer;
	SDL_Window* m_Window;

	Texture m_Texture;
	float* m_DepthBuffer = nullptr;
};