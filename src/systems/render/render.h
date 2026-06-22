#pragma once

#include "../../components/sprite.h"
#include "../../components/transform.h"
#include "../../components/tags.h"

void render_system_process(entt::registry * world, Renderer * renderer, Vec2f screen_size)
{
	auto background_view = world->view<Sprite, Transform, BackgroundTag>();
	for(auto entity : background_view)
	{
		auto & sprite = world->get<Sprite>(entity);
		auto & transform = world->get<Transform>(entity);
		
		float offset = transform.position.x;
		float tile_size = sprite.size.x;

		for (int x = -tile_size; x < screen_size.x + tile_size; x += tile_size)
		{
		    for (int y = 0; y < screen_size.y; y += tile_size)
		    {
		        renderer->draw_texture(sprite.texture,FloatRect{x - offset, static_cast<float>(y), tile_size, tile_size});
		    }
		}
	}

	auto view = world->view<Sprite, Transform>(entt::exclude<BackgroundTag>);
	for(auto entity : view)
	{
		auto & sprite = world->get<Sprite>(entity);
		auto & transform = world->get<Transform>(entity);
		
		renderer->draw_texture(sprite.texture, transform.position.x, transform.position.y, transform.rotation);
	}
}
