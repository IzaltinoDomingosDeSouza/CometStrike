#pragma once

#include "../../vec2.h"

struct Collider
{
	uint32_t layer;
	uint32_t bitmask;

	Vec2f bounds_size;
	bool is_solid;
};
