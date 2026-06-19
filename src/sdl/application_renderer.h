#pragma once

#include "../renderer.h"

#include <SDL3/SDL.h>

class ApplicationRenderer : public Renderer
{
public:
	void init(SDL_Renderer * renderer)
	{
		_renderer = renderer;
		if(!_renderer)
		{
			SDL_Log("SDL_CreateRenderer Error: %s", SDL_GetError());
			SDL_Quit();
			return;
		}
	}
	void shutdown()
	{
		if(_renderer) SDL_DestroyRenderer(_renderer);
		_renderer = nullptr;
	}
	SDL_Renderer * get_native()
	{
		return _renderer;
	}
	void begin()
	{
		SDL_SetRenderDrawColor(_renderer, 0, 0, 0, 255);
		SDL_RenderClear(_renderer);
	}
	void end()
	{
		SDL_RenderPresent(_renderer);
	}
private:
	SDL_Renderer * _renderer = nullptr;
};
