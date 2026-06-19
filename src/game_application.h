#pragma once

#include "renderer.h"

class GameApplication
{
public:
	virtual void init(Renderer * renderer) = 0;
	virtual void update(float delta_time) = 0;
	virtual void render() = 0;
	virtual void shutdown() = 0;
};
