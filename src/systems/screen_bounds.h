#pragma once
#include "../components/transform.h"
#include "../components/sprite.h"

void screen_bounds_system_update(entt::registry * world, Vec2f screen_size)
{
	const float padding = 16.f;	//this make player don't touch the screen
	auto view = world->view<Transform, Sprite, PlayerController>();
	for(auto entity : view)
	{
		auto & transform = view.get<Transform>(entity);
		auto & sprite = view.get<Sprite>(entity);

		const float min = padding;
		const float max = screen_size.y - padding - sprite.size.y;
		transform.position.y = std::clamp(transform.position.y, min, max);
	}
}
