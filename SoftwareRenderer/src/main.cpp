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

	Camera cam;
	cam.position = Vec3(0.0f, 0.0f, 0.0f);
	cam.lookDir = Vec3(0.0f, 1.0f, 0.0f);
	cam.roll = 0.0f;
	cam.yaw = 0.0f;
	cam.pitch = 0.0f;

	while (true)
	{
		if (SDL_PollEvent(&event))
		{
			if (event.type == SDL_QUIT || event.key.keysym.sym == SDLK_ESCAPE)
				break;

			float mult = 1.0f;

			switch (event.key.keysym.sym)
			{
			case SDLK_LEFT:
				cam.position.x += mult;
				break;
			case SDLK_RIGHT:
				cam.position.x -= mult;
				break;
			case SDLK_UP:
				cam.position.y += mult;
				break;
			case SDLK_DOWN:
				cam.position.y -= mult;
				break;
			case SDLK_w:
				cam.position += mult * cam.lookDir;
				break;
			case SDLK_s:
				cam.position -= mult * cam.lookDir;
				break;
			case SDLK_a:
				cam.yaw += mult;
				break;
			case SDLK_d:
				cam.yaw -= mult;
				break;
			case SDLK_q:
				cam.roll -= mult;
				break;
			case SDLK_e:
				cam.roll += mult;
				break;
			case SDLK_z:
				cam.pitch -= mult;
				break;
			case SDLK_c:
				cam.pitch += mult;
				break;



			case SDLK_r:
				cam.position = Vec3(0.0f, 0.0f, 0.0f);
				cam.lookDir = Vec3(0.0f, 1.0f, 0.0f);
				cam.roll = 0.0f;
				cam.yaw = 0.0f;
				cam.pitch = 0.0f;
				break;

			default:
				break;
			}
		}

		renderer.ClearColor(0, 0, 0);
		renderer.Render(cam);
		SDL_RenderPresent(SDLRenderer);
	}

	return 0;
}