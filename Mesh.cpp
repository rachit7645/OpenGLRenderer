#include "Mesh.h"

using namespace Renderer;

Mesh::Mesh
(
	const std::vector<Vertex>& vertices,
	const std::vector<u32>& indices,
	const MeshTextures& textures
)
	: vao(std::make_shared<VertexArray>(vertices, indices)),
	  textures(textures)
{
}