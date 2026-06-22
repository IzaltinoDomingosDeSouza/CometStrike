#pragma once

#include "../components/lifetime.h"
#include "../components/health.h"

void cleanup_system_update(entt::registry * world, float delta_time)
{
	auto lifetime_view = world->view<Lifetime>();
	for(auto entity : lifetime_view)
	{
		auto & lifetime = lifetime_view.get<Lifetime>(entity);

		lifetime.time_remaining -= delta_time;

		if(lifetime.time_remaining <= 0.0f) world->destroy(entity);
	}

	auto health_view = world->view<Health>();
	for(auto entity : health_view)
	{
		auto & health = health_view.get<Health>(entity);

		if(health.amount <= 0.0f) world->destroy(entity);
	}
}
