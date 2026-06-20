#include "../renderer.h"

#include "application_resource_manager.h"
#include "application_renderer.h"

#include <SDL3/SDL.h>

void ApplicationRenderer::init(SDL_Renderer * renderer, ApplicationResourceManager * resource_manager)
{
	_renderer = renderer;
	if(!_renderer)
	{
		SDL_Log("SDL_CreateRenderer Error: %s", SDL_GetError());
		SDL_Quit();
		return;
	}
	
	_resource_manager = resource_manager;
}

void ApplicationRenderer::shutdown()
{
	if(_renderer) SDL_DestroyRenderer(_renderer);
		_renderer = nullptr;
}

SDL_Renderer * ApplicationRenderer::get_native()
{
	return _renderer;
}

void ApplicationRenderer::begin()
{
	SDL_SetRenderDrawColor(_renderer, 0, 0, 0, 255);
	SDL_RenderClear(_renderer);
}

void ApplicationRenderer::end()
{
	SDL_RenderPresent(_renderer);
}
void ApplicationRenderer::draw_texture(TextureHandle handle, float x, float y)
{
	SDL_Texture * internal_texture = _resource_manager->get_texture(handle);

	float width, height;
	SDL_GetTextureSize(internal_texture, &width, &height);

	SDL_FRect dest = { x, y, width, height };
	
	SDL_RenderTexture(_renderer, internal_texture, nullptr, &dest);
}
void ApplicationRenderer::draw_texture(TextureHandle handle, float x, float y, float angle, Flip flip)
{
	SDL_Texture * internal_texture = _resource_manager->get_texture(handle);

	float width, height;
	SDL_GetTextureSize(internal_texture, &width, &height);

	SDL_FRect dest = { x, y, width, height };
	
	SDL_FlipMode internal_flip;

	switch(flip)
	{
		case Flip::Horizontal: internal_flip = SDL_FLIP_HORIZONTAL; break;
		case Flip::Vertical:   internal_flip = SDL_FLIP_VERTICAL;   break;
		default:               internal_flip = SDL_FLIP_NONE;       break;
	}
	SDL_RenderTextureRotated(_renderer, internal_texture, nullptr, &dest, angle, nullptr, internal_flip);
}

void ApplicationRenderer::draw_texture(TextureHandle handle, float x, float y, float origin_x, float origin_y, float angle, Flip flip)
{
	SDL_Texture * internal_texture = _resource_manager->get_texture(handle);

	float width, height;
	SDL_GetTextureSize(internal_texture, &width, &height);

	SDL_FRect dest = { x, y, width, height };
	
	SDL_FPoint origin {origin_x, origin_y};
	
	SDL_FlipMode internal_flip;

	switch(flip)
	{
		case Flip::Horizontal: internal_flip = SDL_FLIP_HORIZONTAL; break;
		case Flip::Vertical:   internal_flip = SDL_FLIP_VERTICAL;   break;
		default:               internal_flip = SDL_FLIP_NONE;       break;
	}
	SDL_RenderTextureRotated(_renderer, internal_texture, nullptr, &dest, angle, &origin, internal_flip);
}
void ApplicationRenderer::draw_texture(TextureHandle handle, FloatRect destination)
{
	SDL_Texture * internal_texture = _resource_manager->get_texture(handle);

	SDL_FRect dest = { destination.x, destination.y, destination.width, destination.height };

	SDL_RenderTexture(_renderer, internal_texture, nullptr, &dest);
}
void ApplicationRenderer::draw_texture(TextureHandle handle, FloatRect source, FloatRect destination)
{
	SDL_Texture * internal_texture = _resource_manager->get_texture(handle);

	SDL_FRect src = { source.x, source.y, source.width, source.height };
	SDL_FRect dest = { destination.x, destination.y, destination.width, destination.height };

	SDL_RenderTexture(_renderer, internal_texture, &src, &dest);
}

