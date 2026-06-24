#pragma once

#include "../../components/sprite.h"
#include "../../components/transform.h"
#include "../../components/tags.h"

void render_system_process(entt::registry * world, Renderer * renderer, ResourceManager * resource_manager, Vec2f screen_size)
{
	auto background_view = world->view<Sprite, Transform, BackgroundTag>();
	for(auto entity : background_view)
	{
		auto & sprite = world->get<Sprite>(entity);
		auto & transform = world->get<Transform>(entity);
		
		float offset = transform.position.x;
		auto texture_info = resource_manager->get_texture_info(sprite.texture);
		Vec2f sprite_size = {static_cast<float>(texture_info.width), static_cast<float>(texture_info.height)};
		float tile_size = sprite_size.x;

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
