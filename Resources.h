#ifndef RESOURCES_H
#define RESOURCES_H

#include <memory>
#include <string>

#include "Texture.h"
#include "Model.h"
#include "Material.h"

namespace Resources
{
	std::shared_ptr<Renderer::Texture> GetTexture(const std::string& path);

	std::shared_ptr<Renderer::Model> GetModel
	(
		const std::string& path,
		const Renderer::MeshTextures& textures,
		const Renderer::Material& material = Renderer::Material()
	);

	void Delete();
}

#endif