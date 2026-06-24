#pragma once

#include <entt/entt.hpp>

#include "../components/sprite.h"
#include "../components/transform.h"
#include "../components/velocity.h"
#include "../components/health.h"
#include "../components/damage.h"
#include "../components/collision/collider.h"

struct CometTemplate
{
	Sprite sprite;
	Transform transform;
	Velocity velocity;
	Health health;
	Damage damage;
	Collider collider;
	
	void create(entt::registry & world)
	{
		auto comet = world.create();

		world.emplace<Sprite>(comet, sprite);
		world.emplace<Transform>(comet, transform);
		world.emplace<Velocity>(comet, velocity);
		world.emplace<Health>(comet, health);
		world.emplace<Damage>(comet, damage);
		world.emplace<Collider>(comet, collider);
	}
};
