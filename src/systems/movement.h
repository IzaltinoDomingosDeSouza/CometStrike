#pragma once
#include "../components/transform.h"
#include "../components/velocity.h"

void movement_system_update(entt::registry * world, float delta_time)
{
	auto view = world->view<Transform, Velocity>();
	for(auto entity : view)
    {
		auto & transform = view.get<Transform>(entity);
		auto & velocity = view.get<Velocity>(entity);

		transform.position.x += velocity.velocity.x * delta_time;
		transform.position.y += velocity.velocity.y * delta_time;
    }
}
