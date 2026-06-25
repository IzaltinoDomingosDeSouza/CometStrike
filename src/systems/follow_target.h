#pragma once

#include "../components/follow_target.h"

void follow_system_update(entt::registry * world)
{
	auto view = world->view<FollowTarget, Transform>(entt::exclude<DisabledTag>);
    for(auto entity : view)
    {
		auto & follow = view.get<FollowTarget>(entity);
		auto & entity_transform = view.get<Transform>(entity);
		
		auto * target_transform = world->try_get<Transform>(follow.target);
		if(follow.active && target_transform)
		{
			entity_transform.position.x = target_transform->position.x + follow.offset.x;
			entity_transform.position.y = target_transform->position.y + follow.offset.y;
		}
    }
}
