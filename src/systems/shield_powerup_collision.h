#pragma once
#include "../components/collision/collision_event.h"
#include "../components/tags.h"

void shield_powerup_collision_system_update(entt::registry * world)
{
	auto view = world->view<CollisionEvent>(entt::exclude<DisabledTag>);
	for(auto entity : view)
	{
		auto & event = view.get<CollisionEvent>(entity);

		if(!world->all_of<ShieldPowerUpTag>(event.target)) continue;

		auto * shield = world->try_get<Shield>(entity);
		if(shield)
		{
			shield->current_capacity += 1;
			if(shield->current_capacity > shield->max_capacity) shield->current_capacity = shield->max_capacity;
			
			auto * shield_health = world->try_get<Health>(shield->shield_entity);
			if(shield_health) shield_health->amount = shield_health->max;
			
			 world->emplace_or_replace<CleanUpTag>(event.target);
		}
	}
}
