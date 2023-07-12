#include "Resources.h"

#include "Log.h"
#include "Files.h"

// Using namespaces
using namespace Engine;

// Usings
using Renderer::Texture;
using Renderer::Model;
using Renderer::MeshTextures;

// Aliases
using TxPtr = Resources::TxPtr;
using MdPtr = Resources::MdPtr;

TxPtr Resources::GetTexture(const std::string& path)
{
    // Try to locate texture
    auto result = m_textures.find(path);

    // If texture was found
    if (result != m_textures.end())
    {
        // Log
        LOG_INFO("Loading cached texture: {}\n", path);
        // Return cached texture
        return result->second;
    }
    // If texture was not found
    else
    {
        // Load texture
        return m_textures[path] = std::make_shared<Texture>(path);
    }
}

MdPtr Resources::GetModel(const std::string& path, const MeshTextures& textures)
{
    // Try to locate model
    auto result = m_models.find(path);

    // If model was found
    if (result != m_models.end())
    {
        // Log
        LOG_INFO("Loading cached model: {}\n", path);
        // Return cached model
        return result->second;
    }
    // If model was not found
    else
    {
        // Load model
        return m_models[path] = std::make_shared<Model>(path, textures);
    }
}

void Resources::Clear()
{
    // Clear textures
    m_textures.clear();
    // Clear models
    m_models.clear();
}

Resources& Resources::GetInstance()
{
    // Static storage
    static Resources resources;
    // Return
    return resources;
}