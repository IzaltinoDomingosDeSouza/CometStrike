#pragma once

#include "renderer.h"
#include "resource_manager.h"

class GameApplication
{
public:
	virtual void init(Renderer * renderer, ResourceManager * resource_manager) = 0;
	virtual void update(float delta_time) = 0;
	virtual void render() = 0;
	virtual void shutdown() = 0;
};
