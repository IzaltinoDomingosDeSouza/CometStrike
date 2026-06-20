#pragma once

#include <string_view>

#include "resource_handle.h"

enum class ResourceType
{
	None = 0,
	Texture = 1
};
struct TextureInfo
{
    int width;
    int height;
};

class ResourceManager
{
public:
    virtual ~ResourceManager() = default;
    
    virtual TextureHandle import_texture(std::string_view filename) = 0;
    virtual TextureInfo get_texture_info(TextureHandle handle) = 0;
};
