#include "Renderer.h"

Renderer::~Renderer()
{
	SDL_DestroyRenderer(m_Renderer);
	SDL_DestroyWindow(m_Window);
	SDL_Quit();
}

void Renderer::Render()
{

}

void Renderer::DrawPixel(int x, int y)
{
	SDL_RenderDrawPoint(m_Renderer, x, y);
}


void Renderer::DrawLine(int x1, int y1, int x2, int y2)
{

}

void Renderer::DrawRectangle(int startX, int startY, int endX, int endY)
{

}

void Renderer::BresenhamNaive(int x1, int y1, int x2, int y2)
{

}

void Renderer::BresenhamHorizontal(int x1, int y1, int x2, int y2)
{

}

void Renderer::BresenhamVertical(int x1, int y1, int x2, int y2)
{

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