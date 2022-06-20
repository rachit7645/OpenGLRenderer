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
		using TX = std::shared_ptr<Texture>;
		MeshTextures(TX& diffuse, TX& specular) : diffuse(diffuse), specular(specular) {}

		TX diffuse;
		TX specular;
	};
}

#endif