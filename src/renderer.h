#pragma once

#include "resource_handle.h"
#include <cstddef>

enum class Flip
{
    None,
    Horizontal,
    Vertical,
    Both = (Horizontal | Vertical)
};
struct FloatRect { float x, y, width, height; };

class Renderer
{
public:
    virtual ~Renderer() = default;
    virtual void draw_texture(TextureHandle handle, float x, float y) = 0;
    virtual void draw_texture(TextureHandle handle, float x, float y, float angle = 0.0f, Flip flip = Flip::None) = 0;
    virtual 	void draw_texture(TextureHandle handle, float x, float y, float origin_x, float origin_y, float angle, Flip flip = Flip::None) = 0;
    virtual void draw_texture(TextureHandle handle, FloatRect destination) = 0;
    virtual void draw_texture(TextureHandle handle, FloatRect source, FloatRect destination) = 0;
};
