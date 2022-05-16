#ifndef MESH_H
#define MESH_H

#include <vector>
#include <memory>

#include "Util.h"
#include "Texture.h"
#include "VertexArray.h"

namespace Renderer
{
	class Mesh
	{
	public:
		Mesh(const std::vector<f32> &vertices, const std::vector<u32> &indices, const std::vector<f32> &txCoords,
			const std::vector<f32> &normals, std::shared_ptr<Texture>& texture);

		std::shared_ptr<VertexArray> vao;
		std::shared_ptr<Texture> texture;
		s32 vertexCount;
	};
}

#endif