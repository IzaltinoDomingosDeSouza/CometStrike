#pragma once

#include "../components/transform.h"
#include "../components/owner.h"
#include "../components/collision/collider.h"
#include "../components/collision/collision_event.h"

void collision_system_update(entt::registry * world)
{
	world->clear<CollisionEvent>();
	auto view = world->view<Collider, Transform>(entt::exclude<DisabledTag>);

	for(auto iterator = view.begin(); iterator != view.end(); ++iterator)
	{
		auto entity1 = *iterator;
		auto & transform1 = view.get<Transform>(entity1);
		auto & collider1 = view.get<Collider>(entity1);

		for(auto next_iterator = std::next(iterator); next_iterator != view.end(); ++next_iterator)
		{
			auto entity2 = *next_iterator;
			auto & transform2 = view.get<Transform>(entity2);
			auto & collider2 = view.get<Collider>(entity2);

			bool can_collide = (collider1.bitmask & collider2.layer) && (collider2.bitmask & collider1.layer);

			auto is_colliding = (transform1.position.x  < transform2.position.x   + collider2.bounds_size.x  &&
								 transform1.position.x  + collider1.bounds_size.x > transform2.position.x    &&
								 transform1.position.y  < transform2.position.y   + collider2.bounds_size.y  &&
								 transform1.position.y  + collider1.bounds_size.y > transform2.position.y);

			if(collider1.is_solid == collider2.is_solid && is_colliding && can_collide)
			{
				auto event_entity = world->create();
				auto & event = world->emplace<CollisionEvent>(event_entity);

				int priority1 = CollisionPriority::get_layer_priority(collider1.layer);
				int priority2 = CollisionPriority::get_layer_priority(collider2.layer);

				if(priority1 >= priority2)
				{
					event.source = entity1;
					event.target = entity2;
				}else
				{
					event.source = entity2;
					event.target = entity1;
				}
			}
		}
	}
}
