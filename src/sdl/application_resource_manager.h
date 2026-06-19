#pragma once

#include "../resource_manager.h"
#include "../renderer.h"
#include "../resource_handle.h"

#include <SDL3/SDL.h>

#include <vector>

class ApplicationResourceManager : public ResourceManager
{
public:
	void init(SDL_Renderer * renderer);
    TextureHandle import_texture(std::string_view filename) override;
    SDL_Texture * get_texture(TextureHandle handle);
    void shutdown();
private:
	std::vector<SDL_Texture*> _textures;
	TextureHandle _texture_count = 1;
	std::vector<TextureHandle> _free_textures;
	SDL_Renderer * _renderer = nullptr;
};
