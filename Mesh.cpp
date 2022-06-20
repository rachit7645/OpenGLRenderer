#include "Mesh.h"

using namespace Renderer;

Mesh::Mesh
(
	const std::vector<f32>& vertices,
	const std::vector<u32>& indices,
	const std::vector<f32>& txCoords,
	const std::vector<f32>& normals,
	const MeshTextures& textures
)
	: vao(std::make_shared<VertexArray>(vertices, indices, txCoords, normals)), textures(textures) {}