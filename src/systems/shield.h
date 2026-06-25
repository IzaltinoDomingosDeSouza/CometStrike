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

	ShieldTemplate sheild_template
	{
		.sprite = {.texture = texture},
		.transform = {.position = {}, .rotation = 90.f},
		.owner = {.entity = {}},
		.follow = {.target = {}, .offset{0.0f, -13.f}, .active = true},
		.health = {.amount = 100, .max = 100},
		.damage = {.amount = 100},
		.collider = {.layer = CollisionLayer::ShieldLayer,
                     .bitmask = CollisionLayer::CometLayer,
                     .bounds_size = texture_size,
                     .is_solid = true},
	};
	//
	
	auto shield_view = world->view<ShieldTag>();
	auto player_view = world->view<Input, Shield, PlayerController>();
	
	for(auto shield_entity : shield_view)
	{
		for(auto player_entity : player_view)
		{
			auto & input = player_view.get<Input>(player_entity);
			auto & shield = player_view.get<Shield>(player_entity);
			if(shield.shield_entity == shield_entity)
			{
				if(input.enable_shield && shield.current_capacity > 1)
				{
					world->remove<DisabledTag>(shield_entity);
				}

				input.enable_shield = false;
			}
		}
	}
	//create shield entity, if player don't have it
	auto view = world->view<Input, Transform, PlayerController>(entt::exclude<Shield>);
	for(auto entity : view)
	{
		auto & transform = view.get<Transform>(entity);
		sheild_template.transform.position = transform.position;
		sheild_template.owner.entity = entity;
		sheild_template.follow.target = entity;

		auto shield_entity = sheild_template.create(*world);

		auto & shield = world->emplace<Shield>(entity);
		shield.shield_entity = shield_entity;
		shield.max_capacity = 3;
		shield.current_capacity = 0;
	}
}
