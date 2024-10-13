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

	Color RepeatSample(float x, float y) const
	{
		float u = x - (float)floor(x);
		float v = y - (float)floor(y);
		int s = (int)((m_Texture->w - 1) * u);
		int t = (int)((m_Texture->h - 1) * v);

		return GetPixelColor(s, t);
	}

	Color ClampSample(float x, float y) const
	{
		x = std::clamp(x, 0.0f, 1.0f);
		y = std::clamp(y, 0.0f, 1.0f);
		int s = (int)((m_Texture->w - 1) * x);
		int t = (int)((m_Texture->h - 1) * y);

		return GetPixelColor(s, t);
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
		if (loadedSurface == nullptr)
		{
			std::cout << "Unable to load image " << filename.c_str() << "SDL_image Error: " << IMG_GetError() << std::endl;
		}
		else
		{
			//Convert surface to screen format
			m_Texture = SDL_ConvertSurface(loadedSurface, windowSurface->format, 0);
			if (m_Texture == nullptr)
			{
				std::cout << "Unable to optimize image " << filename.c_str() << "SDL Error: " << SDL_GetError() << std::endl;
			}

			SDL_FreeSurface(loadedSurface);
		}
	}

	Color GetPixelColor(float x, float y) const
	{
		uint8_t* p = (uint8_t*)m_Texture->pixels + (int)y * m_Texture->pitch + (int)x * 4; // bpp=4

		uint32_t color = *(uint32_t*)p;

		SDL_PixelFormat* fmt = m_Texture->format;

		uint8_t r = (color & fmt->Rmask) >> fmt->Rshift;
		uint8_t g = (color & fmt->Gmask) >> fmt->Gshift;
		uint8_t b = (color & fmt->Bmask) >> fmt->Bshift;

		return { r,g,b };
	}
private:
	SDL_Surface* m_Texture;
};