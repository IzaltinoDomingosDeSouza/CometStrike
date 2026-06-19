#include "application.h"

void Application::run(GameApplication * game)
{
	init();
	game->init(_renderer, _resource_manager);

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

void Application::init()
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
	auto sdl_render = SDL_CreateRenderer(_window, NULL);
	
	_renderer = new ApplicationRenderer;
	_resource_manager = new ApplicationResourceManager;

	_renderer->init(sdl_render, _resource_manager);
	_resource_manager->init(sdl_render);
}

void Application::shutdown()
{
	_renderer->shutdown();
	delete _renderer;
	
	if(_window) SDL_DestroyWindow(_window);

	SDL_Quit();
}
