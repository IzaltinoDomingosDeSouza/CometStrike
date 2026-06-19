#pragma once

#include "application_renderer.h"
#include "application_resource_manager.h"
#include "../game_application.h"

class Application
{
public:
	void run(GameApplication * game);
private:
	SDL_Window * _window;
	ApplicationRenderer * _renderer;
	ApplicationResourceManager * _resource_manager;
	SDL_Event _event;
	bool _is_running;
	
	void init();
	void shutdown();
};
