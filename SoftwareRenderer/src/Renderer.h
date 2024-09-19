#pragma once

#include <SDL.h>

#include "Vec3.h"
#include "Vec2.h"
#include "Mat4x4.h"

class Renderer
{
public:
	Renderer(SDL_Renderer* renderer, SDL_Window* window)
		: m_Renderer(renderer), m_Window(window)
	{
	}

	~Renderer();

	void Render();
	void ClearColor(int r, int g, int b);
	void DrawColor(int r, int g, int b);

	void DrawTriangle(float p1X, float p1Y, float p2X, float p2Y, float p3X, float p3Y);
	//void FillTriangle(Vec2 p1, Vec2 p2, Vec2 p3);

	void DrawRectangle(float startX, float startY, float endX, float endY);

	void DrawLine(float x1, float y1, float x2, float y2);

private:
	void DrawPixel(float x, float y);

	void BresenhamNaive(int x1, int y1, int x2, int y2);
	void BresenhamHorizontal(float x1, float y1, float x2, float y2);
	void BresenhamVertical(float x1, float y1, float x2, float y2);

private:
	void DemoCube();
	void JavidDemo();

private:
	int GetWindowWidth();
	int GetWindowHeight();

private:
	SDL_Renderer* m_Renderer;
	SDL_Window* m_Window;
};