#pragma once

#include <string_view>

#include "resource_handle.h"

enum class ResourceType
{
	None = 0,
	Texture = 1
};

class ResourceManager
{
public:
    virtual ~ResourceManager() = default;
    
    virtual TextureHandle import_texture(std::string_view filename) = 0;
};
