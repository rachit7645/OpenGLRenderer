#ifndef MESH_TEXTURES_H
#define MESH_TEXTURES_H

#include <memory>

#include "Texture.h"

namespace Renderer
{
	class MeshTextures
	{
	public:
		// Usings
		using TxPtr = std::shared_ptr<Texture>;
		// Default Constructor
		MeshTextures() = default;
		// Main Constructor
		MeshTextures
		(
			TxPtr albedo,
			TxPtr normal,
			TxPtr metallic,
			TxPtr roughness,
			TxPtr ao
		);

		// Textures
		TxPtr albedo;
		TxPtr normal;
		TxPtr metallic;
		TxPtr roughness;
		TxPtr ao;
	};
}

#endif