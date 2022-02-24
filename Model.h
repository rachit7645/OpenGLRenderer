#ifndef MODEL_H
#define MODEL_H

#include <vector>
#include <map>
#include <GL/glew.h>
#include "VertexArray.h"
#include "Texture.h"
#include "Util.h"

namespace Renderer
{
	struct Model
	{
		Model(std::vector<f32>& m_vertices, std::vector<u32>& m_indices, std::vector<f32>& m_textureCoords, const std::string& texturePath);
		std::vector<f32> vertices;				 // Vertices
		std::vector<u32> indices;				 // Indices
		std::vector<f32> textureCoords;			 // Texture Coordinates
		VertexArray vao;
		Texture texture;						 // Texture
	};
}

#endif // MODEL_H