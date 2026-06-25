#pragma once
#include "../components/transform.h"
#include "../components/sprite.h"

void screen_bounds_system_update(entt::registry * world, ResourceManager * resource_manager, Vec2f screen_size)
{
	const float padding = 16.f;	//this make player don't touch the screen
	auto view = world->view<Transform, Sprite, PlayerController>(entt::exclude<DisabledTag>);
	for(auto entity : view)
	{
		auto & transform = view.get<Transform>(entity);
		auto & sprite = view.get<Sprite>(entity);
		
		auto texture_info = resource_manager->get_texture_info(sprite.texture);
		Vec2f sprite_size = {static_cast<float>(texture_info.width), static_cast<float>(texture_info.height)};

		const float min = padding;
		const float max = screen_size.y - padding - sprite_size.y;
		transform.position.y = std::clamp(transform.position.y, min, max);
	}
}
