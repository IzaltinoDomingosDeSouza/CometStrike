#pragma once

#include <string_view>

#include "../renderer.h"
#include "application_resource_manager.h"

#include <SDL3/SDL.h>

class ApplicationRenderer : public Renderer
{
public:
	void init(SDL_Renderer * renderer, ApplicationResourceManager * resource_manager);
	void shutdown();
	SDL_Renderer * get_native();
	void begin();
	void end();
	void draw_texture(TextureHandle handle, float x, float y) override;
	void draw_texture(TextureHandle handle, float x, float y, float angle = 0.0f, Flip flip = Flip::None) override;
	void draw_texture(TextureHandle handle, float x, float y, float origin_x, float origin_y, float angle, Flip flip = Flip::None) override;
	void draw_texture(TextureHandle handle, FloatRect destination) override;
    void draw_texture(TextureHandle handle, FloatRect source, FloatRect destination) override;
private:
	SDL_Renderer * _renderer = nullptr;
	ApplicationResourceManager * _resource_manager;
};
