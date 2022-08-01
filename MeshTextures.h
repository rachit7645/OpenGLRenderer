#ifndef MESH_TEXTURES_H
#define MESH_TEXTURES_H

#include <memory>

#include "Texture.h"
#include "Util.h" 

namespace Renderer
{
	class MeshTextures
	{
	public:
		using TxPtr = std::shared_ptr<Texture>;

		// Default Constructor
		MeshTextures() = default;
		// Main Constructor
		MeshTextures(TxPtr diffuse, TxPtr specular)
			: diffuse(std::move(diffuse)),
			  specular(std::move(specular))
		{
		}

		TxPtr diffuse;
		TxPtr specular;
	};
}

#endif