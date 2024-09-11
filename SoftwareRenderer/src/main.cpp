#define SDL_MAIN_HANDLED
#include <SDL.h>

#include "Renderer.h"

#define SCREEN_WIDTH 600

int main()
{
	SDL_Event event;
	SDL_Renderer* SDLRenderer;
	SDL_Window* window;

	SDL_Init(SDL_INIT_VIDEO);
	SDL_CreateWindowAndRenderer(SCREEN_WIDTH, SCREEN_WIDTH, 0, &window, &SDLRenderer);
	SDL_SetRenderDrawColor(SDLRenderer, 0, 0, 0, 0);
	SDL_RenderClear(SDLRenderer);
	SDL_SetRenderDrawColor(SDLRenderer, 0, 255, 255, 255);

	Renderer renderer(SDLRenderer, window);
	renderer.Render();

	while (true)
	{
		if (SDL_PollEvent(&event) && event.type == SDL_QUIT || event.type == SDL_KEYDOWN)
			break;
	}

	return 0;
}