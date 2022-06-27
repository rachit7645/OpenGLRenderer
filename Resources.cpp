#include "Resources.h"

using Renderer::Texture;
using Renderer::PathType;
using Renderer::Model;
using Renderer::MeshTextures;
using Renderer::Material;

using TxPtr = std::shared_ptr<Texture>;
using MdPtr = std::shared_ptr<Model>;

// Note: This implementation is not the fastest, but it works very well

// FIXME: Remove std::string from resource maps in this file
// Reasons:
// 1. Unnecessary allocations
// 2. Copying strings
// 3. Construction/Destruction

std::unordered_map<std::string, TxPtr> m_textures;
std::unordered_map<std::string, MdPtr> m_models;

TxPtr Resources::GetTexture(const std::string& path, PathType pathType)
{
	auto result = m_textures.find(path);
	if (result != m_textures.end())
	{
		LOG_INFO("Loading texture: ", Files::GetRelative(path), "\n");
		return result->second;
	}
	else
	{
		return m_textures[path] = std::make_shared<Texture>(path, pathType);
	}
	// Hello cosmic ray
	return nullptr;
}

MdPtr Resources::GetModel(const std::string& path, const MeshTextures& textures, const Material& material)
{
	auto result = m_models.find(path);
	if (result != m_models.end())
	{
		LOG_INFO("Loading model: ", Files::GetRelative(path), "\n");
		return result->second;
	}
	else
	{
		return m_models[path] = std::make_shared<Model>(path, textures, material);
	}
	// Hello cosmic ray (again)
	return nullptr;
}