#pragma once

#include <entt/entt.hpp>

#include "../components/sprite.h"
#include "../components/transform.h"
#include "../components/owner.h"
#include "../components/health.h"
#include "../components/damage.h"
#include "../components/follow_target.h"
#include "../components/collision/collider.h"
#include "../components/tags.h"

struct ShieldTemplate
{
	Sprite sprite;
	Transform transform;
	Owner owner;
	FollowTarget follow;
	Health health;
	Damage damage;
	Collider collider;

	entt::entity create(entt::registry & world)
	{
		auto entity = world.create();

		world.emplace<Sprite>(entity, sprite);
		world.emplace<Transform>(entity, transform);
		world.emplace<ShieldTag>(entity, ShieldTag{});
		world.emplace<Owner>(entity, owner);
		world.emplace<FollowTarget>(entity, follow);
		world.emplace<Health>(entity, health);
		world.emplace<Damage>(entity, damage);
		world.emplace<Collider>(entity, collider);
		world.emplace<DisabledTag>(entity, collider);
		return entity;
	}
};
