#ifndef TEXTURE_CACHE_H
#define TEXTURE_CACHE_H

#include <unordered_map>
#include <memory>
#include <string>

#include "Texture.h"
#include "Util.h"
#include "Log.h"
#include "Files.h"

namespace TextureCache
{
	std::shared_ptr<Renderer::Texture> GetTexture(const std::string& path, Renderer::PathType pathType = Renderer::PathType::RELATIVE);
}

#endif