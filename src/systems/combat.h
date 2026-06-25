#pragma once
#include "../components/collision/collision_event.h"
#include "../components/health.h"
#include "../components/damage.h"

void combat_system_update(entt::registry * world)
{
	auto view = world->view<CollisionEvent>(entt::exclude<DisabledTag>);
	for(auto entity : view)
	{
		auto & event = view.get<CollisionEvent>(entity);

		auto * health = world->try_get<Health>(entity);
		auto * damage = world->try_get<Damage>(event.target);
		
		auto * shield = world->try_get<Shield>(entity);
		if(shield && damage)
		{
			auto * shield_health = world->try_get<Health>(shield->shield_entity);
			
			shield_health->amount -= damage->amount;
			if(shield_health->amount <= 0)
			{
				shield->current_capacity -= 1;
				world->emplace_or_replace<DisabledTag>(shield->shield_entity);
				shield_health->amount = shield_health->max;
			}
		}else if(health && damage)
		{
			health->amount -= damage->amount;
		}
	}
}
