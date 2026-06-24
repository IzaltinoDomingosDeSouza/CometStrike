#include "atsin2d/application_layer/application.h"
#include "atsin2d/game_application.h"

#include <entt/entt.hpp>

#include <utility>

#include "vec2.h"
#include "collision_layer.h"

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
#include "systems/shield.h"

#include "systems/input.h"
#include "systems/control.h"
#include "systems/shield.h"
#include "systems/collision.h"
#include "systems/combat.h"
#include "systems/movement.h"
#include "systems/shoot.h"
#include "systems/comet_wave.h"
#include "systems/screen_bounds.h"
#include "systems/background_scrolling.h"
#include "systems/cleanup.h"
#include "systems/render/render.h"
#include "systems/follow_target.h"

enum class CometType : size_t
{
	Tiny = 0,
	Small,
	Med,
	Big
};
constexpr size_t COMET_TYPE_COUNT = 4;

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

		_world.emplace<BackgroundTag>(background);

		auto player = _world.create();
		auto & player_controller = _world.emplace<PlayerController>(player);
		player_controller.index = 0;

		auto & player_sprite = _world.emplace<Sprite>(player);
		player_sprite.texture = resource_manager->import_texture("assets/spaceship.png");

		auto player_texture_info = resource_manager->get_texture_info(player_sprite.texture);
		Vec2f player_sprite_size = {static_cast<float>(player_texture_info.width), static_cast<float>(player_texture_info.height)};
		
		auto & player_transform = _world.emplace<Transform>(player);
		player_transform.position = {16.0f, screen_size.y * 0.5f - player_sprite_size.y * 0.5f};
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
		player_health.max = 100.f;

		auto & player_damage = _world.emplace<Damage>(player);
		player_damage.amount = 100;

		auto & collider = _world.emplace<Collider>(player);
		collider.layer = CollisionLayer::PlayerLayer;
		collider.bitmask = CollisionLayer::CometLayer | CollisionLayer::ProjectileLayer;
		collider.bounds_size = player_sprite_size;
		collider.is_solid = true;

		//avoid memory allocation on update
		_projectile_texture = resource_manager->import_texture("assets/projectile.png");
		_comet_texture[std::to_underlying(CometType::Tiny)] = resource_manager->import_texture("assets/comets/tiny1.png");
		_comet_texture[std::to_underlying(CometType::Small)] = resource_manager->import_texture("assets/comets/small1.png");
		_comet_texture[std::to_underlying(CometType::Med)] = resource_manager->import_texture("assets/comets/med1.png");
		_comet_texture[std::to_underlying(CometType::Big)] = resource_manager->import_texture("assets/comets/big1.png");
		_shield_texture = resource_manager->import_texture("assets/shield.png");

		auto comet_wave = _world.create();
		auto & wave = _world.emplace<Wave>(comet_wave);
		wave.timer = 0;
		wave.spawn_rate = 2.f;
	}
	void update(float delta_time) override
	{
		input_system_update(&_world);
		control_system_update(&_world);
		shield_system_update(&_world, _resource_manager, _shield_texture);
		collision_system_update(&_world);
		combat_system_update(&_world);
		movement_system_update(&_world, delta_time);
		shoot_system_update(&_world, delta_time,_projectile_texture,_resource_manager);
		comet_wave_system_update(&_world, delta_time, _resource_manager, _comet_texture, screen_size);
		screen_bounds_system_update(&_world, _resource_manager, screen_size);
		follow_system_update(&_world);
		background_scrolling_system_update(&_world, delta_time);
		cleanup_system_update(&_world, delta_time);
	}
	void render() override
	{	
		render_system_process(&_world, _renderer, _resource_manager, screen_size);
	}
	void shutdown() override
	{
	
	}
private:
	Renderer * _renderer;
	ResourceManager * _resource_manager;
	entt::registry _world;

	const Vec2f screen_size{800, 600};	//TODO it need to add an api

	TextureHandle _projectile_texture;
	TextureHandle _comet_texture[COMET_TYPE_COUNT];
	TextureHandle _shield_texture;
};
int main()
{
	CometStrike game;
	
	Application app;
	app.run(&game);
}
