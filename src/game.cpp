#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

#include "sdl/application.h"
#include "game_application.h"

struct Vec2f { float x; float y; };

class CometStrike : public GameApplication
{
public:
	void init(Renderer * renderer,  ResourceManager * resource_manager) override
	{
		_renderer = renderer;
		
		background_texture = resource_manager->import_texture("assets/background.png");
		player_texture = resource_manager->import_texture("assets/spaceship.png");
		
		position = Vec2f{0,0};
	}
	void update(float delta_time) override
	{
		const bool * key_state = SDL_GetKeyboardState(NULL);
		
		if(key_state[SDL_SCANCODE_W])
		{
			position.y -= speed * delta_time;
		}
		if(key_state[SDL_SCANCODE_S])
		{
			position.y += speed * delta_time;
		}
	}
	void render() override
	{
		_renderer->draw_texture(background_texture, {0,0,256,256}, {0,0,800,600});
		_renderer->draw_texture(player_texture, position.x, position.y);
	}
	void shutdown() override
	{
	
	}
private:
	Renderer * _renderer;
	TextureHandle player_texture;
	TextureHandle background_texture;
	
	Vec2f position;
	float speed = 200;
};
int main()
{
	CometStrike game;
	
	Application app;
	app.run(&game);
}
