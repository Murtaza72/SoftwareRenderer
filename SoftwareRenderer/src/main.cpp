#include "pch.h"

#include "Renderer.h"

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 800;

int HandleInput(SDL_Event event, float elapsedTime, Camera& cam);

int main()
{
	SDL_Event event = SDL_Event();

	Renderer renderer(SCREEN_WIDTH, SCREEN_HEIGHT);

	Texture t(SDL_GetWindowSurface(renderer.GetWindow()), "./assets/me.jpg");
	renderer.SetTexture(t);

	Light light;
	light.dir = { 1.0f,1.0f,-1.0f };
	light.color = Colors::White;
	renderer.SetLightSource(light);

	float nearPlane = 0.1f;
	float farPlane = 1000.0f;
	float fov = 90.0f;
	float aspectRatio = (float)SCREEN_WIDTH / SCREEN_HEIGHT;
	renderer.SetProjection(fov, aspectRatio, nearPlane, farPlane);

	int flags = RENDER_TEXTURED;

	Mesh cubeMesh;
	if (flags & RENDER_TEXTURED)
	{
		cubeMesh.LoadCube();
	}

	if (flags & RENDER_FLAT)
	{
		if (!cubeMesh.LoadObject("./assets/bunny.obj"), false)
			std::cout << "Could not load the obj file!!!" << std::endl;
	}

	Camera cam;
	cam.position = Vec3(0.0f, 0.0f, 0.0f);
	cam.lookDir = Vec3(0.0f, 0.0f, 1.0f);
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
		std::chrono::duration<float> time = now - start;
		start = now;
		float elapsedTime = time.count();

		if (HandleInput(event, elapsedTime, cam) == 0)
			break;

		{
			#define ROTATE

			float theta = 0;
			#ifdef ROTATE
			theta += SDL_GetTicks() / 2000.0f;
			#else
			theta = 0.0f;
			#endif

			float scale = 5.0f;

			Mat4x4 rotationMatX = Mat4x4::RotationX(theta);
			Mat4x4 rotationMatY = Mat4x4::RotationY(theta);
			Mat4x4 rotationMatZ = Mat4x4::RotationZ(theta);
			Mat4x4 translateMat = Mat4x4::Translation(0.0f, 0.0f, 5.0f);
			Mat4x4 scaleMat = Mat4x4::Scale(scale, scale, scale);

			Mat4x4 worldMat = scaleMat * rotationMatX * rotationMatY * rotationMatZ * translateMat;
			renderer.SetTransform(worldMat);
		}

		{
			Vec3 up(0.0f, 1.0f, 0.0f);
			Vec3 target(0.0f, 0.0f, 1.0f);

			Mat4x4 camRotation = Mat4x4::Identity();
			camRotation = camRotation * Mat4x4::RotationY(cam.yaw);
			//camRotation = camRotation * Mat4x4::RotationX(cam.roll);
			//camRotation = camRotation * Mat4x4::RotationZ(cam.pitch);
			cam.lookDir = MultiplyMatrixVector(target, camRotation);
			target = cam.position + cam.lookDir;

			renderer.SetCamera(cam.position, target, up);
		}

		{
			renderer.ClearColor(Colors::Black);
			renderer.ClearDepth();
			renderer.Render(cubeMesh, cam, flags);
			renderer.Present();
		}

		float frameTime = SDL_GetTicks() - startTimer;
		float fps = (frameTime > 0) ? 1000.0f / frameTime : 0.0f;
		std::string title = "Software Renderer FPS: " + std::to_string((int)fps) + " Frame Time: " + std::to_string(frameTime) + " ms";
		SDL_SetWindowTitle(renderer.GetWindow(), title.c_str());
	}

	return 0;
}

int HandleInput(SDL_Event event, float elapsedTime, Camera& cam)
{
	if (SDL_PollEvent(&event))
	{
		if (event.type == SDL_QUIT || event.key.keysym.sym == SDLK_ESCAPE)
			return 0;

		float mult = 10.0f * -elapsedTime;

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
			cam.roll += mult;
			break;
		case SDLK_e:
			cam.roll -= mult;
			break;
		case SDLK_z:
			cam.pitch += mult;
			break;
		case SDLK_c:
			cam.pitch -= mult;
			break;

		case SDLK_r:
			cam.position = Vec3(0.0f, 0.0f, 0.0f);
			cam.lookDir = Vec3(0.0f, 0.0f, 1.0f);
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