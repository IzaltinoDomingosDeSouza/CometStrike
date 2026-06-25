#pragma once

enum CollisionLayer : uint32_t
{
    NoneLayer       = 0,
    PlayerLayer     = 1 << 0,
    CometLayer      = 1 << 1,
    ProjectileLayer = 1 << 2,
    PoweUpLayer     = 1 << 3,
    ShieldLayer     = 1 << 4,
};
namespace CollisionPriority
{
	size_t get_layer_priority(uint32_t layer)
	{
		switch(layer)
		{
		    case CollisionLayer::ProjectileLayer: return 10;
		    case CollisionLayer::PoweUpLayer:     return 8;
		    case CollisionLayer::ShieldLayer:     return 5;
		    case CollisionLayer::PlayerLayer:     return 2;
		    case CollisionLayer::CometLayer:      return 1;
		    default:                              return 0;
		}
	}
}
