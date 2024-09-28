#define SDL_MAIN_HANDLED
#include <SDL.h>
#include <chrono>

#include "Renderer.h"

const int SCREEN_WIDTH = 900;
const int SCREEN_HEIGHT = 900;

int HandleInput(SDL_Event event, float elapsedTime, Camera& cam);

int main()
{
	SDL_Event event = SDL_Event();
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

	auto start = std::chrono::system_clock::now();

	while (true)
	{
		uint32_t startTimer;

		startTimer = SDL_GetTicks();

		// Handle Timing
		auto now = std::chrono::system_clock::now();
		std::chrono::duration<float> time = start - now;
		start = now;
		float elapsedTime = time.count();

		if (HandleInput(event, elapsedTime, cam) == 0)
			break;

		{
			renderer.ClearColor(0, 0, 0);
			renderer.Render(cam);
			SDL_RenderPresent(SDLRenderer);
		}

		uint32_t frameTime = SDL_GetTicks() - startTimer;
		float fps = (frameTime > 0) ? 1000.0f / frameTime : 0.0f;
		printf("FPS: %f\r", fps);
	}

	return 0;
}

int HandleInput(SDL_Event event, float elapsedTime, Camera& cam)
{
	if (SDL_PollEvent(&event))
	{
		if (event.type == SDL_QUIT || event.key.keysym.sym == SDLK_ESCAPE)
			return 0;

		float mult = 10.0f * elapsedTime;

		switch (event.key.keysym.sym)
		{
		case SDLK_LEFT:
			cam.position.x -= mult;
			break;
		case SDLK_RIGHT:
			cam.position.x += mult;
			break;
		case SDLK_UP:
			cam.position.y -= mult;
			break;
		case SDLK_DOWN:
			cam.position.y += mult;
			break;
		case SDLK_w:
			cam.position -= mult * cam.lookDir;
			break;
		case SDLK_s:
			cam.position += mult * cam.lookDir;
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

	return 1;
}