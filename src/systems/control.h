#pragma once
#include "../components/movement.h"
#include "../components/velocity.h"

void control_system_update(entt::registry * world)
{
	auto view = world->view<Movement, Velocity>(entt::exclude<DisabledTag>);
	for(auto entity : view)
    {
		auto & movement = view.get<Movement>(entity);
		auto & velocity = view.get<Velocity>(entity);

		velocity.velocity.x = movement.direction.x * movement.speed;
		velocity.velocity.y = movement.direction.y * movement.speed;
    }
}
