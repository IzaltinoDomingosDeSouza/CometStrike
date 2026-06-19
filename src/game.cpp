#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

#include "sdl/application.h"
#include "game_application.h"

class CometStrike : public GameApplication
{
public:
	void init(Renderer * renderer) override
	{
		_renderer = renderer;
	}
	void update(float delta_time) override
	{
		
	}
	void render() override
	{
	
	}
	void shutdown() override
	{
	
	}
private:
	Renderer * _renderer;
};
int main()
{
	CometStrike game;
	
	Application app;
	app.run(&game);
}
