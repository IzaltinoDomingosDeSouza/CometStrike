#pragma once
#include "../components/input.h"
#include "../components/transform.h"
#include "../components/sprite.h"
#include "../components/projectile.h"

#include <functional>

void shoot_system_update(entt::registry * world, float delta_time, std::function<void(entt::registry &, ResourceManager *,
																	TextureHandle, Owner,Transform, Velocity, Damage)>
																	create_projectile,
																	TextureHandle projectile_texture,
																	ResourceManager * resource_manager)
{
	auto view = world->view<Input, Transform, Sprite, Projectile>();
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

			create_projectile(*world, resource_manager, projectile_texture, Owner{.entity = entity} ,
							  projectile_transform, Velocity {100,0}, Damage{.amount = 10});

			input.shoot = false;

			projectile.cooldown_timer = projectile.fire_rate;
		}
	}
}
