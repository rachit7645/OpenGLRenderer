#include "Model.h"

using namespace Renderer;

Model::Model(std::vector<f32>& m_vertices,
			 std::vector<u32>& m_indices,
			 std::vector<f32>& m_textureCoords,
			 const std::string& texturePath) 
			: vertices{m_vertices},
			 indices{m_indices},
			 textureCoords{m_textureCoords},
			 vao(vertices, indices, textureCoords),
			 texture(texturePath) {}