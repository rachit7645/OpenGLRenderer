#ifndef MESH_H
#define MESH_H

#include <vector>
#include <memory>

#include "VertexArray.h"
#include "MeshTextures.h"
#include "Material.h"

namespace Renderer
{
	class Mesh
	{
	public:
		using VAO = std::shared_ptr<VertexArray>;
		Mesh
		(
			const std::vector<f32>& vertices,
			const std::vector<u32>& indices,
			const std::vector<f32>& txCoords,
			const std::vector<f32>& normals,
			const MeshTextures& textures,
			const Material& material
		);

		VAO          vao;
		MeshTextures textures;
		Material     material;
	};
}

#endif