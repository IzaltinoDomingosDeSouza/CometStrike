#pragma once
#include "../components/input.h"
#include "../components/transform.h"
#include "../components/sprite.h"
#include "../components/projectile.h"

#include "../entity_template/projectile.h"
#include "../collision_layer.h"

void shoot_system_update(entt::registry * world, float delta_time, TextureHandle projectile_texture, ResourceManager * resource_manager)
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
			auto projectile_texture_info = resource_manager->get_texture_info(projectile_texture);
			auto sprite_texture_info = resource_manager->get_texture_info(sprite.texture);

			Vec2f projectile_size = {static_cast<float>(projectile_texture_info.width), static_cast<float>(projectile_texture_info.height)};
			Vec2f sprite_size = {static_cast<float>(sprite_texture_info.width), static_cast<float>(sprite_texture_info.height)};

			Vec2f position ={transform.position.x + (sprite_size.x - projectile_size.x) * 0.5f,
							 transform.position.y + (sprite_size.y - projectile_size.y) * 0.5f};

			ProjectileTemplate projectile_template = {
				.sprite {.texture = projectile_texture},
				.transform {.position = position,
							.rotation = 90.0f},
				.lifetime {.time_remaining = 6.f},
				.velocity {.velocity = {100.f, 0.0f}},
				.owner {.entity = entity},
				.damage {.amount = 10},
				.collider {.layer = CollisionLayer::ProjectileLayer,
				           .bitmask = CollisionLayer::CometLayer,
						   .bounds_size = projectile_size,
						   .is_solid = true},
			};

		
			projectile_template.create(*world);

			input.shoot = false;

			projectile.cooldown_timer = projectile.fire_rate;
		}
	}
}
