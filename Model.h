#ifndef MODEL_H
#define MODEL_H

#include <vector>

#include "VertexArray.h"
#include "Texture.h"
#include "Log.h"
#include "Util.h"

namespace Renderer
{
	struct Model
	{
		Model(std::vector<f32>& vert, std::vector<u32>& indi, std::vector<f32>& txCoords, std::vector<f32> &norms, Texture& tx);
		std::vector<f32> vertices;				// Vertices
		std::vector<u32> indices;				// Indices
		std::vector<f32> textureCoords;			// Texture Coordinates
		std::vector<f32> normals;				// Normals
		VertexArray vao;						// Vertex Attribute Objects
		Texture texture;						// Texture
		f32 shineDamper = 1.0f;
		f32 reflectivity = 0.0f;
	};

	Model LoadModel(const std::string& mPath, Texture& texture);
}

#endif // MODEL_H