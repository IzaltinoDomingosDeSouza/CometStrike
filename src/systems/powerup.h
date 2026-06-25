#pragma once
#include "../components/powerup.h"

void powerup_system_update(entt::registry * world, float delta_time, ResourceManager * resource_manager, TextureHandle texture)
{
	const float spawn_rate = 8.f;
	static float countdown_timer = spawn_rate; 
	countdown_timer -= delta_time;

	auto texture_info = resource_manager->get_texture_info(texture);
	Vec2f texture_size = {static_cast<float>(texture_info.width), static_cast<float>(texture_info.height)};

	PowerUpTemplate powerup_template
	{
		.sprite = {.texture = texture},
		.transform = {.position = {800 + texture_size.x, 300}, .rotation = 0.0f},
		.velocity = {-100.0f, 0.0f},
		.collider = {.layer = CollisionLayer::PoweUpLayer,
                     .bitmask = CollisionLayer::PlayerLayer,
                     .bounds_size = texture_size,
                     .is_solid = true},
	};
	
	if(countdown_timer <= 0.0f)
	{
		powerup_template.create(*world, PowerUpType::ShieldPowerUp);
		countdown_timer = spawn_rate;
	}
}
