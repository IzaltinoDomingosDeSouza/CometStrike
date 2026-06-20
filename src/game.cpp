#include "atsin2d/application_layer/application.h"
#include "atsin2d/game_application.h"

#include <entt/entt.hpp>

struct Vec2f { float x; float y; };

struct Transform
{
	Vec2f position;
	float rotation;
};
struct Sprite
{
	TextureHandle texture;
	Vec2f size;
};

struct BackgroundTag {};

struct Input
{
    bool up;
    bool down;
};

struct Velocity
{
    Vec2f velocity;
};
struct Player
{
    int index;
};
class CometStrike : public GameApplication
{
public:
	void init(Renderer * renderer,  ResourceManager * resource_manager) override
	{
		_renderer = renderer;
		
		_background = _world.create();
		auto & background_transform = _world.emplace<Transform>(_background);
		background_transform.position = {0,0};
		background_transform.rotation = 0;
		
		auto & background_sprite = _world.emplace<Sprite>(_background);
		background_sprite.texture = resource_manager->import_texture("assets/background.png");
		auto background_info = resource_manager->get_texture_info(background_sprite.texture);
		background_sprite.size = {static_cast<float>(background_info.width), static_cast<float>(background_info.height)};
		
		_world.emplace<BackgroundTag>(_background);
		
		
		_player = _world.create();
		auto player = _world.emplace<Player>(_player);
		player.index = 0;

		auto & player_sprite = _world.emplace<Sprite>(_player);
		player_sprite.texture = resource_manager->import_texture("assets/spaceship.png");
		auto player_texture_info = resource_manager->get_texture_info(player_sprite.texture);
		player_sprite.size = {static_cast<float>(player_texture_info.width), static_cast<float>(player_texture_info.height)};

		auto & player_transform = _world.emplace<Transform>(_player);
		player_transform.position = {16.0f, screen_size.y * 0.5f - player_sprite.size.y * 0.5f};
		player_transform.rotation = -90;

		auto & player_input = _world.emplace<Input>(_player);
		player_input.up = false;
		player_input.down = false;

		auto & player_velocity = _world.emplace<Velocity>(_player);
		player_velocity.velocity = {0.0f, 0.0f};
	}
	void update(float delta_time) override
	{
		// input system
		{
			const bool * key_state = SDL_GetKeyboardState(NULL);
			
			auto view = _world.view<Input, Player>();
			for(auto entity : view)
		    {
		        auto & input = view.get<Input>(entity);
		        auto & player = view.get<Player>(entity);
		        
				if(player.index == 0)
				{
					input.up = key_state[SDL_SCANCODE_W];
					input.down = key_state[SDL_SCANCODE_S];
					
				}else if(player.index == 1)
				{
					input.up = key_state[SDL_SCANCODE_UP];
					input.down = key_state[SDL_SCANCODE_DOWN];
				}
				//Test only to create a way to game application quit
				if(key_state[SDL_SCANCODE_ESCAPE]) request_quit();
			}
		}
		//movement system
		{
			auto view = _world.view<Transform, Velocity, Input>();
			for(auto entity : view)
		    {
				auto & transform = view.get<Transform>(entity);
				auto & velocity = view.get<Velocity>(entity);
				auto & input = view.get<Input>(entity);

				const float speed = 100.0f;

				velocity.velocity.y = 0.0f;

				if(input.up)   velocity.velocity.y -= speed;
				if(input.down) velocity.velocity.y += speed;
				
				transform.position.x += velocity.velocity.x * delta_time;
				transform.position.y += velocity.velocity.y * delta_time;
		    }
		}
		//screen bounds system
		{
			const float padding = 16.f;	//this make player don't touch the screen
			auto view = _world.view<Transform, Sprite, Player>();
			for(auto entity : view)
			{
				auto & transform = view.get<Transform>(entity);
				auto & sprite = view.get<Sprite>(entity);

				const float min = padding;
				const float max = screen_size.y - padding - sprite.size.y;
				transform.position.y = std::clamp(transform.position.y, min, max);
			}
		}
		//background scrolling system
		{
			auto view = _world.view<Transform, BackgroundTag>();
			for(auto entity : view)
		    {
		        auto & transform = view.get<Transform>(entity);

		        transform.position.x += 100.f * delta_time;

		        // wrap safely
		        transform.position.x = std::fmod(transform.position.x, 256.f);
		        if(transform.position.x < 0) transform.position.x += 256.f;
		    }
        }
	}
	void render() override
	{	
		auto background_view = _world.view<Sprite, Transform, BackgroundTag>();
		for(auto entity : background_view)
		{
			auto & sprite = _world.get<Sprite>(entity);
			auto & transform = _world.get<Transform>(entity);
			
			float offset = transform.position.x;
			float tile_size = sprite.size.x;

            for (int x = -tile_size; x < screen_size.x + tile_size; x += tile_size)
            {
                for (int y = 0; y < screen_size.y; y += tile_size)
                {
                    _renderer->draw_texture(sprite.texture,FloatRect{x - offset, y, tile_size, tile_size});
                }
            }
		}

		auto view = _world.view<Sprite, Transform>(entt::exclude<BackgroundTag>);
		for(auto entity : view)
		{
			auto & sprite = _world.get<Sprite>(entity);
			auto & transform = _world.get<Transform>(entity);
			
			_renderer->draw_texture(sprite.texture, transform.position.x, transform.position.y, transform.rotation);
		}
	}
	void shutdown() override
	{
	
	}
private:
	Renderer * _renderer;
	entt::registry _world;
	entt::entity _background;
	entt::entity _player;

	const Vec2f screen_size{800, 600};	//TODO it need to add an api
};
int main()
{
	CometStrike game;
	
	Application app;
	app.run(&game);
}
