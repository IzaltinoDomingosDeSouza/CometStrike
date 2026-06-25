#pragma once

#include <entt/entt.hpp>

#include "../components/sprite.h"
#include "../components/transform.h"
#include "../components/velocity.h"
#include "../components/collision/collider.h"
#include "../components/tags.h"

enum class PowerUpType
{
    ShieldPowerUp
};

struct PowerUpTemplate
{
	Sprite sprite;
	Transform transform;
	Velocity velocity;
	Collider collider;

	entt::entity create(entt::registry & world, PowerUpType type)
	{
		auto entity = world.create();

		world.emplace<Sprite>(entity, sprite);
		world.emplace<Transform>(entity, transform);
		world.emplace<Velocity>(entity, velocity);
		world.emplace<Collider>(entity, collider);
		//TODO use switch when add more types
		if(type == PowerUpType::ShieldPowerUp) world.emplace<ShieldPowerUpTag>(entity, ShieldPowerUpTag{});
		return entity;
	}
};
