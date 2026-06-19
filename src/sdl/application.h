#pragma once

#include "application_renderer.h"
#include "../game_application.h"

class Application
{
public:
	void run(GameApplication * game)
	{
		init();
		game->init(_renderer);

		while(_is_running)
		{
			while(SDL_PollEvent(&_event))
			{
				if(_event.type == SDL_EVENT_QUIT) _is_running = false;
			}

			float delta_time = 0;
			
			game->update(delta_time);
			
			_renderer->begin();
			game->render();
			_renderer->end();
		}

		game->shutdown();
		
		shutdown();
	}
private:
	SDL_Window * _window;
	ApplicationRenderer * _renderer;
	SDL_Event _event;
	bool _is_running;
	
	void init()
	{
		_is_running = true;

		if(!SDL_Init(SDL_INIT_VIDEO))
		{
			SDL_Log("SDL_Init Error: %s", SDL_GetError());
			return;
		}

		_window = SDL_CreateWindow("Comet Strike", 800, 600, 0);
		if(!_window)
		{
			SDL_Log("SDL_CreateWindow Error: %s", SDL_GetError());
			SDL_Quit();
			return;
		}
		_renderer = new ApplicationRenderer;
		_renderer->init(SDL_CreateRenderer(_window, NULL));
	}
	void shutdown()
	{
		_renderer->shutdown();
		delete _renderer;
		if(_window) SDL_DestroyWindow(_window);

		SDL_Quit();
	}
};
