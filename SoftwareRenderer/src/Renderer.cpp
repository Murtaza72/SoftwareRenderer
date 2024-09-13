#include "Renderer.h"

#include <iostream>
#include <chrono>
#include <thread>
#include <time.h>
#include <stdlib.h>

Renderer::~Renderer()
{
	SDL_DestroyRenderer(m_Renderer);
	SDL_DestroyWindow(m_Window);
	SDL_Quit();
}

void Renderer::Render()
{
	DemoCube();
}

void Renderer::DrawPixel(int x, int y)
{
	SDL_RenderDrawPoint(m_Renderer, x, y);
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

void Renderer::DrawLine(int x1, int y1, int x2, int y2)
{
	if (abs(x2 - x1) > abs(y2 - y1))
		BresenhamHorizontal(x1, y1, x2, y2);
	else
		BresenhamVertical(x1, y1, x2, y2);

	SDL_RenderPresent(m_Renderer);
	std::this_thread::sleep_for(std::chrono::milliseconds(50));

}

void Renderer::DrawRectangle(int startX, int startY, int endX, int endY)
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

void Renderer::BresenhamHorizontal(int x1, int y1, int x2, int y2)
{
	// invert y-axis
	y1 = GetWindowHeight() - y1;
	y2 = GetWindowHeight() - y2;

	// Swap em
	if (x1 > x2)
	{
		int temp = x1;
		x1 = x2;
		x2 = temp;

		temp = y1;
		y1 = y2;
		y2 = temp;
	}

	int x = x1;
	int y = y1;
	int dx = x2 - x1;
	int dy = y2 - y1;

	int p = 2 * dy - dx;

	int dir = dy < 0 ? -1 : 1;
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

void Renderer::BresenhamVertical(int x1, int y1, int x2, int y2)
{
	// invert y-axis
	y1 = GetWindowHeight() - y1;
	y2 = GetWindowHeight() - y2;

	// Swap em
	if (y1 > y2)
	{
		int temp = x1;
		x1 = x2;
		x2 = temp;

		temp = y1;
		y1 = y2;
		y2 = temp;
	}

	int x = x1;
	int y = y1;
	int dx = x2 - x1;
	int dy = y2 - y1;

	int p = 2 * dx - dy;

	int dir = dx < 0 ? -1 : 1;
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

	while (true)
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