#include "Resources.h"

#include <unordered_map>

#include "Log.h"
#include "Files.h"

using Renderer::Texture;
using Renderer::Model;
using Renderer::MeshTextures;
using Renderer::Material;

using namespace Resources;

// Note: This implementation is not the fastest, but it works very well

std::unordered_map<std::string, TxPtr> m_textures;
std::unordered_map<std::string, MdPtr> m_models;

TxPtr Resources::GetTexture(const std::string& path)
{
	auto result = m_textures.find(path);
	if (result != m_textures.end())
	{
		LOG_INFO("Loading cached texture: ", path, "\n");
		return result->second;
	}
	else
	{
		return m_textures[path] = std::make_shared<Texture>(path);
	}
}

MdPtr Resources::GetModel
(
	const std::string& path,
	const MeshTextures& textures,
	const Material& material
)
{
	auto result = m_models.find(path);
	if (result != m_models.end())
	{
		LOG_INFO("Loading cached model: ", path, "\n");
		return result->second;
	}
	else
	{
		return m_models[path] = std::make_shared<Model>(path, textures, material);
	}
}

void Resources::Delete()
{
	m_textures.clear();
	m_models.clear();
}