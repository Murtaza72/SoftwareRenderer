#pragma once

#include "pch.h"

class Texture
{
public:
	Texture()
	{
	}

	Texture(SDL_Surface* windowSurface, std::string filename)
		: m_Texture(nullptr)
	{
		if (Init())
		{
			LoadSurface(windowSurface, filename);
		}
	}

	Color GetRGB(float x, float y)
	{
		x *= m_Texture->w;
		y *= m_Texture->h;
		Uint32 color = GetPixel(x, y);

		SDL_PixelFormat* fmt = m_Texture->format;

		uint8_t r = (color & fmt->Rmask) >> fmt->Rshift;
		uint8_t g = (color & fmt->Gmask) >> fmt->Gshift;
		uint8_t b = (color & fmt->Bmask) >> fmt->Bshift;

		return { r,g,b };
	}

private:
	int Init()
	{
		int flag = IMG_INIT_PNG;
		if (!IMG_Init(flag))
		{
			std::cout << "SDL Image could not be initialized" << std::endl;
			return 0;
		}

		return 1;
	}

	void LoadSurface(SDL_Surface* windowSurface, std::string filename)
	{
		SDL_Surface* loadedSurface = IMG_Load(filename.c_str());
		if (loadedSurface == NULL)
		{
			std::cout << "Unable to load image " << filename.c_str() << "SDL_image Error: " << IMG_GetError() << std::endl;
		}
		else
		{
			//Convert surface to screen format
			m_Texture = SDL_ConvertSurface(loadedSurface, windowSurface->format, 0);
			if (m_Texture == NULL)
			{
				std::cout << "Unable to optimize image " << filename.c_str() << "SDL Error: " << SDL_GetError() << std::endl;
			}

			SDL_FreeSurface(loadedSurface);
		}
	}

	uint32_t GetPixel(float x, float y)
	{
		Uint8* p = (Uint8*)m_Texture->pixels + (int)y * m_Texture->pitch + (int)x * 4; // bpp=4

		return *(Uint32*)p;
	}
private:
	SDL_Surface* m_Texture;
};