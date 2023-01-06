#ifndef MESH_H
#define MESH_H

#include <vector>
#include <memory>

#include "VertexArray.h"
#include "MeshTextures.h"
#include "Vertex.h"

namespace Renderer
{
	class Mesh
	{
	public:
		// Usings
		using VAO = std::shared_ptr<VertexArray>;

		// Main constructor
		Mesh
		(
			const std::vector<Vertex>& vertices,
			const std::vector<u32>& indices,
			const Renderer::MeshTextures& textures
		);

		// Vertex Attribute Object
		VAO vao;
		// Textures
		MeshTextures textures;
	};
}

#endif