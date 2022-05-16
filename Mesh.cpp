#include "Mesh.h"

using namespace Renderer;

Mesh::Mesh(const std::vector<f32> &vertices, const std::vector<u32> &indices, const std::vector<f32> &txCoords,
	const std::vector<f32> &normals, std::shared_ptr<Texture>& texture)
	: texture{ texture }, vertexCount{ static_cast<s32>(indices.size()) }
{
	vao = std::make_shared<VertexArray>(vertices, indices, txCoords, normals);
}