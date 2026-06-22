#pragma once

#include "../components/tags.h"
#include "../components/transform.h"

void background_scrolling_system_update(entt::registry * world, float delta_time)
{
	auto view = world->view<Transform, BackgroundTag>();
	for(auto entity : view)
    {
        auto & transform = view.get<Transform>(entity);

        transform.position.x += 100.f * delta_time;

        // wrap safely
        transform.position.x = std::fmod(transform.position.x, 256.f);
        if(transform.position.x < 0) transform.position.x += 256.f;
    }
}
