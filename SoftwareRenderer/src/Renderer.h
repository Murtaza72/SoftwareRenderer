#pragma once

#include <SDL.h>

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

	void DrawRectangle(int startX, int startY, int endX, int endY);

	void DrawLine(int x1, int y1, int x2, int y2);

private:
	void DrawPixel(int x, int y);

	void BresenhamNaive(int x1, int y1, int x2, int y2);
	void BresenhamHorizontal(int x1, int y1, int x2, int y2);
	void BresenhamVertical(int x1, int y1, int x2, int y2);

private:
	void DemoCube();

private:
	int GetWindowWidth();
	int GetWindowHeight();

private:
	SDL_Renderer* m_Renderer;
	SDL_Window* m_Window;
};