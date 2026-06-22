#include "atsin2d/application_layer/application.h"
#include "atsin2d/game_application.h"

#include <entt/entt.hpp>

#include <random>
#include <utility>

#include "vec2.h"
#include "components/tags.h"
#include "components/transform.h"
#include "components/sprite.h"
#include "components/input.h"
#include "components/velocity.h"
#include "components/movement.h"
#include "components/player_controller.h"
#include "components/projectile.h"
#include "components/lifetime.h"
#include "components/wave.h"
#include "components/health.h"
#include "components/damage.h"
#include "components/owner.h"
#include "components/collision/collider.h"
#include "components/collision/collision_event.h"

enum class CometType : size_t
{
	Tiny = 0,
	Small,
	Med,
	Big
};
constexpr size_t COMET_TYPE_COUNT = 4;

//projectile factory
void create_projectile(entt::registry & world, ResourceManager * resource_manager, TextureHandle texture, Owner owner,
					  Transform transform, Velocity velocity,  Damage damage)
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

	auto & collider = world.emplace<Collider>(projectile);
	collider.bounds_size = sprite.size;
	collider.is_solid = true;

	world.emplace<Damage>(projectile, damage);
	world.emplace<Owner>(projectile, owner);
}
void create_comet(entt::registry & world, ResourceManager * resource_manager, TextureHandle texture,
				 Transform transform, Velocity velocity, Health health, Damage damage)
{
	auto comet = world.create();

	auto & sprite = world.emplace<Sprite>(comet);
	sprite.texture = texture;
	auto texture_info = resource_manager->get_texture_info(sprite.texture);
	sprite.size = {static_cast<float>(texture_info.width), static_cast<float>(texture_info.height)};

	world.emplace<Transform>(comet, transform);
	world.emplace<Velocity>(comet, velocity);
	world.emplace<Health>(comet, health);
	world.emplace<Damage>(comet, damage);

	auto & collider = world.emplace<Collider>(comet);
	collider.bounds_size = sprite.size;
	collider.is_solid = true;
}
class CometStrike : public GameApplication
{
public:
	void init(Renderer * renderer,  ResourceManager * resource_manager) override
	{
		_renderer = renderer;
		_resource_manager = resource_manager;
		
		auto background = _world.create();
		auto & background_transform = _world.emplace<Transform>(background);
		background_transform.position = {0,0};
		background_transform.rotation = 0;

		auto & background_sprite = _world.emplace<Sprite>(background);
		background_sprite.texture = resource_manager->import_texture("assets/background.png");
		auto background_info = resource_manager->get_texture_info(background_sprite.texture);
		background_sprite.size = {static_cast<float>(background_info.width), static_cast<float>(background_info.height)};

		_world.emplace<BackgroundTag>(background);

		auto player = _world.create();
		auto & player_controller = _world.emplace<PlayerController>(player);
		player_controller.index = 0;

		auto & player_sprite = _world.emplace<Sprite>(player);
		player_sprite.texture = resource_manager->import_texture("assets/spaceship.png");
		auto player_texture_info = resource_manager->get_texture_info(player_sprite.texture);
		player_sprite.size = {static_cast<float>(player_texture_info.width), static_cast<float>(player_texture_info.height)};

		auto & player_transform = _world.emplace<Transform>(player);
		player_transform.position = {16.0f, screen_size.y * 0.5f - player_sprite.size.y * 0.5f};
		player_transform.rotation = -90;

		auto & player_input = _world.emplace<Input>(player);
		player_input.shoot = false;

		auto & player_velocity = _world.emplace<Velocity>(player);
		player_velocity.velocity = {0.0f, 0.0f};

		auto & player_movement = _world.emplace<Movement>(player);
		player_movement.direction = {0.0f, 0.0f};
		player_movement.speed = 100;

		auto & player_projectile = _world.emplace<Projectile>(player);
		player_projectile.cooldown_timer = 0;
		player_projectile.fire_rate = 0.6f;

		auto & player_health = _world.emplace<Health>(player);
		player_health.amount = 100;
		player_health.max = 0.6f;

		auto & player_damage = _world.emplace<Damage>(player);
		player_damage.amount = 100;

		auto & collider = _world.emplace<Collider>(player);
		collider.bounds_size = player_sprite.size;
		collider.is_solid = true;

		//avoid memory allocation on update
		_projectile_texture = resource_manager->import_texture("assets/projectile.png");
		_comet_texture[std::to_underlying(CometType::Tiny)] = resource_manager->import_texture("assets/comets/tiny1.png");
		_comet_texture[std::to_underlying(CometType::Small)] = resource_manager->import_texture("assets/comets/small1.png");
		_comet_texture[std::to_underlying(CometType::Med)] = resource_manager->import_texture("assets/comets/med1.png");
		_comet_texture[std::to_underlying(CometType::Big)] = resource_manager->import_texture("assets/comets/big1.png");

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

			auto view = _world.view<Input, Movement, PlayerController>();
			for(auto entity : view)
		    {
		        auto & input = view.get<Input>(entity);
		        auto & movement = view.get<Movement>(entity);
		        auto & player = view.get<PlayerController>(entity);

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
		//collision system
		{
			auto view = _world.view<Collider, Transform>();

			for(auto iterator = view.begin(); iterator != view.end(); ++iterator)
			{
				auto entity1 = *iterator;
				auto & transform1 = view.get<Transform>(entity1);
				auto & collider1 = view.get<Collider>(entity1);

				for(auto next_iterator = std::next(iterator); next_iterator != view.end(); ++next_iterator)
				{
					auto entity2 = *next_iterator;
					auto & transform2 = view.get<Transform>(entity2);
					auto & collider2 = view.get<Collider>(entity2);

					auto is_colliding = (transform1.position.x  < transform2.position.x   + collider2.bounds_size.x  &&
										 transform1.position.x  + collider1.bounds_size.x > transform2.position.x    &&
										 transform1.position.y  < transform2.position.y   + collider2.bounds_size.y  &&
										 transform1.position.y  + collider1.bounds_size.y > transform2.position.y);

					if(collider1.is_solid == collider2.is_solid && is_colliding)
					{
						auto & event1 = _world.emplace_or_replace<CollisionEvent>(entity1);
						event1.target = entity2;

						auto & event2 = _world.emplace_or_replace<CollisionEvent>(entity2);
						event2.target = entity1;
					}
				}
			}
		}
		//combat system
		{
			auto view = _world.view<CollisionEvent>();
			for(auto entity : view)
			{
				auto & event = view.get<CollisionEvent>(entity);

				auto * owner = _world.try_get<Owner>(event.target);
				
				if(owner && owner->entity == entity) continue;

				auto * health = _world.try_get<Health>(entity);
				auto * damage = _world.try_get<Damage>(event.target);

				if(health && damage)
				{
					health->amount -= damage->amount;
				}
			}
			_world.clear<CollisionEvent>();
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

					create_projectile(_world, _resource_manager, _projectile_texture, Owner{.entity = entity} ,
									  projectile_transform, Velocity {100,0}, Damage{.amount = 10});

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
					float comet_damage[] = {10.0f, 25.0f, 75.0f, 100.0f};

					//Tiny = 50%, Small = 30%, Med = 15%, Big = 5%
					std::vector<double> frequency = {50.0, 30.0, 15.0, 5.0};
					std::discrete_distribution<size_t> comet_type_distribuition(frequency.begin(), frequency.end());

					size_t comet_type = comet_type_distribuition(_random_engine);
					auto texture = _comet_texture[comet_type];

					auto texture_info = _resource_manager->get_texture_info(texture);
					std::uniform_real_distribution<float> comet_position_distribuition(texture_info.height, screen_size.y - texture_info.height);

					Vec2f comet_position = {screen_size.x + texture_info.width, comet_position_distribuition(_random_engine)};
					create_comet(_world,_resource_manager, texture,
								Transform{comet_position, 0}, Velocity{-100, 0.f},
								Health{100.f,100.f}, Damage{.amount = comet_damage[comet_type]});
					wave.timer = wave.spawn_rate;
				}
			}
		}
		//screen bounds system
		{
			const float padding = 16.f;	//this make player don't touch the screen
			auto view = _world.view<Transform, Sprite, PlayerController>();
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
		//clean up system for Health and Lifetime entities
		{
			auto lifetime_view = _world.view<Lifetime>();
			for(auto entity : lifetime_view)
			{
				auto & lifetime = lifetime_view.get<Lifetime>(entity);

				lifetime.time_remaining -= delta_time;

				if(lifetime.time_remaining <= 0.0f) _world.destroy(entity);
			}

			auto health_view = _world.view<Health>();
			for(auto entity : health_view)
			{
				auto & health = health_view.get<Health>(entity);

				if(health.amount <= 0.0f) _world.destroy(entity);
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

	std::mt19937 _random_engine;

	const Vec2f screen_size{800, 600};	//TODO it need to add an api

	TextureHandle _projectile_texture;
	TextureHandle _comet_texture[COMET_TYPE_COUNT];
};
int main()
{
	CometStrike game;
	
	Application app;
	app.run(&game);
}
