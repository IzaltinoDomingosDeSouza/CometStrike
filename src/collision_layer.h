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
