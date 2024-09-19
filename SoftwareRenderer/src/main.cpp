#define SDL_MAIN_HANDLED
#include <SDL.h>

#include "Renderer.h"

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 800;

int main()
{
	SDL_Event event;
	SDL_Renderer* SDLRenderer;
	SDL_Window* window;

	SDL_Init(SDL_INIT_VIDEO);
	SDL_CreateWindowAndRenderer(SCREEN_WIDTH, SCREEN_HEIGHT, 0, &window, &SDLRenderer);

	Renderer renderer(SDLRenderer, window);

	while (true)
	{
		if (SDL_PollEvent(&event) && event.type == SDL_QUIT || event.type == SDL_KEYDOWN)
			break;

		renderer.ClearColor(0, 0, 0);
		renderer.DrawColor(255, 0, 255);
		renderer.Render();
		SDL_RenderPresent(SDLRenderer);
	}

	return 0;
}