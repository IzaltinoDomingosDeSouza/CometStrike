#pragma once

#include <entt/entt.hpp>

#include "../components/sprite.h"
#include "../components/transform.h"
#include "../components/lifetime.h"
#include "../components/velocity.h"
#include "../components/owner.h"
#include "../components/damage.h"
#include "../components/collision/collider.h"

struct ProjectileTemplate
{
	Sprite sprite;
	Transform transform;
	Lifetime lifetime;
	Velocity velocity;
	Owner owner;
	Damage damage;
	Collider collider;
	
	void create(entt::registry & world)
	{
		auto projectile = world.create();

		world.emplace<Sprite>(projectile, sprite);
		world.emplace<Transform>(projectile, transform);
		world.emplace<Velocity>(projectile, velocity);
		world.emplace<Lifetime>(projectile, lifetime);
		world.emplace<Collider>(projectile, collider);
		world.emplace<Damage>(projectile, damage);
		world.emplace<Owner>(projectile, owner);
	}
};
