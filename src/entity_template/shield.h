#pragma once

#include <entt/entt.hpp>

#include "../components/sprite.h"
#include "../components/transform.h"
#include "../components/shield.h"
#include "../components/owner.h"
#include "../components/health.h"
#include "../components/damage.h"
#include "../components/follow_target.h"
#include "../components/collision/collider.h"

struct ShieldTemplate
{
	Sprite sprite;
	Transform transform;
	Shield shield;
	Owner owner;
	FollowTarget follow;
	Health health;
	Damage damage;
	Collider collider;

	void create(entt::registry & world)
	{
		auto entity = world.create();

		world.emplace<Sprite>(entity, sprite);
		world.emplace<Transform>(entity, transform);
		world.emplace<Shield>(entity, shield);
		world.emplace<Owner>(entity, owner);
		world.emplace<FollowTarget>(entity, follow);
		world.emplace<Health>(entity, health);
		world.emplace<Damage>(entity, damage);
		world.emplace<Collider>(entity, collider);
	}
};
