#ifndef RESOURCES_H
#define RESOURCES_H

#include <memory>
#include <string>
#include <unordered_map>

#include "Texture.h"
#include "Model.h"

namespace Engine
{
	class Resources
	{
	private:
		// Main constructor
		Resources() = default;
	public:
		// Usings
		using TxPtr = std::shared_ptr<Renderer::Texture>;
		using MdPtr = std::shared_ptr<Renderer::Model>;
		// If the same texture exists, it returns it (fast)
		// Otherwise it generates it (slow)
		TxPtr GetTexture(const std::string& path);
		// If the same model exists, it returns it (fast)
		// Otherwise it generates it (slow)
		MdPtr GetModel(const std::string& path, const Renderer::MeshTextures& textures);
		// Clears the cache
		void Delete();
	private:
		// Texture map
		std::unordered_map<std::string, TxPtr> m_textures;
		// Model map
		std::unordered_map<std::string, MdPtr> m_models;
	public:
		// Singleton getter
		static Resources& GetInstance();
	};
}

#endif