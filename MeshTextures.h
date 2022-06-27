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
		MeshTextures(TxPtr& diffuse, TxPtr& specular) : diffuse(diffuse), specular(specular) {}

		TxPtr diffuse;
		TxPtr specular;
	};
}

#endif