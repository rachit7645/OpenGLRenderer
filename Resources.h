#ifndef RESOURCES_H
#define RESOURCES_H

#include <unordered_map>
#include <memory>
#include <string>

#include "Texture.h"
#include "Util.h"
#include "Log.h"
#include "Files.h"
#include "Model.h"
#include "Material.h"

namespace Resources
{
	std::shared_ptr<Renderer::Texture> GetTexture
	(
		const std::string& path,
		Renderer::PathType pathType = Renderer::PathType::RELATIVE
	);

	std::shared_ptr<Renderer::Model> GetModel
	(
		const std::string& path,
		const Renderer::MeshTextures& textures,
		const Renderer::Material& material = Renderer::Material()
	)
	;
	void Delete();
}

#endif