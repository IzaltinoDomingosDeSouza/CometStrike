#include "../resource_manager.h"
#include "../renderer.h"
#include "../resource_handle.h"

#include "application_resource_manager.h"
#include "application_renderer.h"

#include <SDL3_image/SDL_image.h>
#include <SDL3_ttf/SDL_ttf.h>

#include <vector>

void ApplicationResourceManager::init(SDL_Renderer * renderer)
{
	_renderer = renderer;
}

TextureHandle ApplicationResourceManager::import_texture(std::string_view filename)
{
	TextureHandle handle = InvalidTexture;
		
	SDL_Texture * texture = IMG_LoadTexture(_renderer, filename.data());

	if(texture)
	{
		if(!_free_textures.empty())
		{
			handle = _free_textures.back();
			_free_textures.pop_back();
			_textures[handle] = texture;
		}else
		{
		
			if(_texture_count >= _textures.size())
				_textures.resize(_texture_count + 1);
			
			handle = _texture_count++;
			_textures[handle] = texture;
		}
	}
		
	return handle;
}
SDL_Texture * ApplicationResourceManager::get_texture(TextureHandle handle)
{
	//static_assert(handle > _texture_count);
	return _textures[handle];
}

void ApplicationResourceManager::shutdown()
{
	for(SDL_Texture * texture :  _textures)
	{
		if(texture) SDL_DestroyTexture(texture);
	}
	_textures.clear();
}
