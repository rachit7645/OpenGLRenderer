#include "TextureCache.h"

using Renderer::Texture;
using Renderer::PathType;
using TxPtr = std::shared_ptr<Texture>;

// Note: This implementation is not the fastest, but it works very well

// FIXME: Remove std::string
// Reasons:
// 1. Unnecessary allocations
// 2. Copying strings
// 3. Construction/Destruction
std::unordered_map<std::string, TxPtr> textures;

TxPtr TextureCache::GetTexture(const std::string& path, PathType pathType)
{
	auto result = textures.find(path);
	if (result != textures.end())
	{
		LOG_INFO("Loading texture: ", Files::GetRelative(path), "\n");
		return result->second;
	}
	else
	{
		return textures[path] = std::make_shared<Texture>(path, pathType);
	}
	// Hello cosmic ray
	return nullptr;
}