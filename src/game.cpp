#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

#include "sdl/application.h"
#include "game_application.h"

class CometStrike : public GameApplication
{
public:
	void init(Renderer * renderer,  ResourceManager * resource_manager) override
	{
		_renderer = renderer;
		
		background_texture = resource_manager->import_texture("assets/background.png");
		player_texture = resource_manager->import_texture("assets/spaceship.png");
	}
	void update(float delta_time) override
	{
		
	}
	void render() override
	{
		_renderer->draw_texture(background_texture, {0,0,256,256}, {0,0,800,600});
		_renderer->draw_texture(player_texture, 100, 100);
	}
	void shutdown() override
	{
	
	}
private:
	Renderer * _renderer;
	TextureHandle player_texture;
	TextureHandle background_texture;
};
int main()
{
	CometStrike game;
	
	Application app;
	app.run(&game);
}
