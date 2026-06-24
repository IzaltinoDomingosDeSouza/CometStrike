#pragma once
#include "../components/input.h"
#include "../components/transform.h"
#include "../components/player_controller.h"
#include "../components/shield.h"

#include "../entity_template/shield.h"

#include "../collision_layer.h"

void shield_system_update(entt::registry * world, ResourceManager * resource_manager, TextureHandle texture)
{
	auto texture_info = resource_manager->get_texture_info(texture);
	Vec2f texture_size = {static_cast<float>(texture_info.width), static_cast<float>(texture_info.height)};

	auto shield_view = world->view<Shield, Owner>();
    auto player_view = world->view<Input, Transform, PlayerController>();
    for(auto player_entity : player_view)
    {
        auto & input = player_view.get<Input>(player_entity);
        if(input.enable_shield)
        {
        		for(auto shield_entity : shield_view)
        		{
        			auto & owner = shield_view.get<Owner>(shield_entity);
        			auto & shield = shield_view.get<Shield>(shield_entity);
        			if(owner.entity == player_entity)
        			{
        				shield.activate = true;
        			}
        		}
        }
    }

    for(auto player_entity : player_view)
    {
    		auto & transform = player_view.get<Transform>(player_entity);
    		bool has = false;
		for(auto shield_entity : shield_view)
		{
			auto & owner = shield_view.get<Owner>(shield_entity);
			if(owner.entity == player_entity)
			{
				has = true;
				break;
			}
		}
		if(!has)
		{
			ShieldTemplate sheild_template
			{
				.sprite = {.texture = texture},
				.transform = {.position = {transform.position.x + 200.f, transform.position.y}, .rotation = 90.f},
				.shield = {.activate = false},
				.owner = {.entity = player_entity},
				.follow = {.target = player_entity, .offset{0.0f, -13.f}, .active = true},
				.health = {.amount = 100, .max = 100},
				.damage = {.amount = 100},
				.collider = {.layer = CollisionLayer::ShieldLayer,
                             .bitmask = CollisionLayer::CometLayer,
                             .bounds_size = texture_size,
                             .is_solid = false},
			};
			sheild_template.create(*world);
    		}
    }
}
