#pragma once
#include "../components/collision/collision_event.h"
#include "../components/health.h"
#include "../components/damage.h"

void combat_system_update(entt::registry * world)
{
	auto view = world->view<CollisionEvent>();
	for(auto entity : view)
	{
		auto & event = view.get<CollisionEvent>(entity);

		auto * owner = world->try_get<Owner>(event.target);
		
		if(owner && owner->entity == entity) continue;

		auto * health = world->try_get<Health>(entity);
		auto * damage = world->try_get<Damage>(event.target);

		if(health && damage)
		{
			health->amount -= damage->amount;
		}
	}
	world->clear<CollisionEvent>();
}
