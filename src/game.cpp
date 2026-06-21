#include "atsin2d/application_layer/application.h"
#include "atsin2d/game_application.h"

#include <entt/entt.hpp>

#include <random>

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
    bool shoot;
};

struct Velocity
{
    Vec2f velocity;
};

struct Movement
{
	Vec2f direction;
	float speed;
};
struct Player
{
    int index;
};

struct Projectile
{
	float cooldown_timer;
	float fire_rate;
};

struct Lifetime
{
	float time_remaining;
};

struct Wave
{
	float timer;
	float spawn_rate;
};

//projectile factory
void create_projectile(entt::registry & world, ResourceManager * resource_manager, TextureHandle texture, Transform transform, Velocity velocity)
{
	auto projectile = world.create();

	auto & sprite = world.emplace<Sprite>(projectile);
	sprite.texture = texture;
	auto texture_info = resource_manager->get_texture_info(sprite.texture);
	sprite.size = {static_cast<float>(texture_info.width), static_cast<float>(texture_info.height)};

	//center
	transform.position.x -= sprite.size.x * 0.5;
	transform.position.y -= sprite.size.y * 0.5;
	world.emplace<Transform>(projectile, transform);

	world.emplace<Velocity>(projectile, velocity);

	auto & lifetime = world.emplace<Lifetime>(projectile);
	lifetime.time_remaining = 6.f;
}
void create_comet(entt::registry & world, ResourceManager * resource_manager, TextureHandle texture, Transform transform, Velocity velocity)
{
	auto comet = world.create();

	auto & sprite = world.emplace<Sprite>(comet);
	sprite.texture = texture;
	auto texture_info = resource_manager->get_texture_info(sprite.texture);
	sprite.size = {static_cast<float>(texture_info.width), static_cast<float>(texture_info.height)};

	world.emplace<Transform>(comet, transform);
	world.emplace<Velocity>(comet, velocity);
}
class CometStrike : public GameApplication
{
public:
	void init(Renderer * renderer,  ResourceManager * resource_manager) override
	{
		_renderer = renderer;
		_resource_manager = resource_manager;
		
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
		auto & player = _world.emplace<Player>(_player);
		player.index = 0;

		auto & player_sprite = _world.emplace<Sprite>(_player);
		player_sprite.texture = resource_manager->import_texture("assets/spaceship.png");
		auto player_texture_info = resource_manager->get_texture_info(player_sprite.texture);
		player_sprite.size = {static_cast<float>(player_texture_info.width), static_cast<float>(player_texture_info.height)};

		auto & player_transform = _world.emplace<Transform>(_player);
		player_transform.position = {16.0f, screen_size.y * 0.5f - player_sprite.size.y * 0.5f};
		player_transform.rotation = -90;

		auto & player_input = _world.emplace<Input>(_player);
		player_input.shoot = false;

		auto & player_velocity = _world.emplace<Velocity>(_player);
		player_velocity.velocity = {0.0f, 0.0f};

		auto & player_movement = _world.emplace<Movement>(_player);
		player_movement.direction = {0.0f, 0.0f};
		player_movement.speed = 100;

		auto & player_projectile = _world.emplace<Projectile>(_player);
		player_projectile.cooldown_timer = 0;
		player_projectile.fire_rate = 0.6f;

		//avoid memory allocation on update
		_projectile_texture = resource_manager->import_texture("assets/projectile.png");
		_comet_texture = resource_manager->import_texture("assets/comets/tiny1.png");

		auto comet_wave = _world.create();
		auto & wave = _world.emplace<Wave>(comet_wave);
		wave.timer = 0;
		wave.spawn_rate = 2.f;
	}
	void update(float delta_time) override
	{
		// input system
		{
			const bool * key_state = SDL_GetKeyboardState(NULL);

			//Test only to create a way to game application quit
			if(key_state[SDL_SCANCODE_ESCAPE]) request_quit();

			auto view = _world.view<Input, Movement, Player>();
			for(auto entity : view)
		    {
		        auto & input = view.get<Input>(entity);
		        auto & movement = view.get<Movement>(entity);
		        auto & player = view.get<Player>(entity);

		        movement.direction = {0.f,0.f};
		        
		        bool up = false;
		        bool down = false;
				if(player.index == 0)
				{
					up = key_state[SDL_SCANCODE_W];
					down = key_state[SDL_SCANCODE_S];
					input.shoot = key_state[SDL_SCANCODE_SPACE];
					
				}else if(player.index == 1)
				{
					up = key_state[SDL_SCANCODE_UP];
					down = key_state[SDL_SCANCODE_DOWN];
					input.shoot = key_state[SDL_SCANCODE_RSHIFT];
				}
				if(up)	   movement.direction.y -= 1.f;
				if(down) movement.direction.y += 1.f;
			}
		}
		//control system
		{
			auto view = _world.view<Movement, Velocity>();
			for(auto entity : view)
		    {
				auto & movement = view.get<Movement>(entity);
				auto & velocity = view.get<Velocity>(entity);

				velocity.velocity.x = movement.direction.x * movement.speed;
				velocity.velocity.y = movement.direction.y * movement.speed;
		    }
		}
		//movement system
		{
			auto view = _world.view<Transform, Velocity>();
			for(auto entity : view)
		    {
				auto & transform = view.get<Transform>(entity);
				auto & velocity = view.get<Velocity>(entity);

				transform.position.x += velocity.velocity.x * delta_time;
				transform.position.y += velocity.velocity.y * delta_time;
		    }
		}
		//shoot system
		{
			auto view = _world.view<Input, Transform, Sprite, Projectile>();
			for(auto entity : view)
			{
				auto & transform = view.get<Transform>(entity);
				auto & input = view.get<Input>(entity);
				auto & sprite = view.get<Sprite>(entity);
				auto & projectile = view.get<Projectile>(entity);

				projectile.cooldown_timer -= delta_time;

				if(input.shoot && projectile.cooldown_timer <= 0.0f)
				{
					Vec2f center = {transform.position.x + (sprite.size.x * 0.5f) , transform.position.y + (sprite.size.y * 0.5f)};
					Transform projectile_transform = {.position = center, .rotation = 90};

					create_projectile(_world, _resource_manager, _projectile_texture, projectile_transform, {100,0});

					input.shoot = false;

					projectile.cooldown_timer = projectile.fire_rate;
				}
			}
		}
		//comet wave system
		{
			auto view = _world.view<Wave>();
			for(auto entity : view)
			{
				auto & wave = view.get<Wave>(entity);

				wave.timer -= delta_time;

				if(wave.timer <= 0.0f)
				{
					auto texture_info = _resource_manager->get_texture_info(_comet_texture);
					std::uniform_real_distribution<float> distribuition(texture_info.height, screen_size.y - texture_info.height);

					Vec2f comet_position = {screen_size.x + texture_info.width, distribuition(_random_engine)};
					create_comet(_world, _resource_manager, _comet_texture, {comet_position, 0}, {-150.f, 0.f});
					wave.timer = wave.spawn_rate;
				}
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
		//Lifetime
		{
			auto view = _world.view<Lifetime>();
			for(auto entity : view)
			{
				auto & lifetime = view.get<Lifetime>(entity);

				lifetime.time_remaining -= delta_time;

				if(lifetime.time_remaining <= 0.0f) _world.destroy(entity);
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
                    _renderer->draw_texture(sprite.texture,FloatRect{x - offset, static_cast<float>(y), tile_size, tile_size});
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
	ResourceManager * _resource_manager;
	entt::registry _world;
	entt::entity _background;
	entt::entity _player;

	std::mt19937 _random_engine;

	const Vec2f screen_size{800, 600};	//TODO it need to add an api

	TextureHandle _projectile_texture;
	TextureHandle _comet_texture;
};
int main()
{
	CometStrike game;
	
	Application app;
	app.run(&game);
}
