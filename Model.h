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
		Model(VertexArray vao, Texture& tx, size_t vertex_count);
		VertexArray vao;						// Vertex Attribute Object
		Texture texture;						// Texture
		f32 shineDamper = 1.0f;					// Shine Dampner
		f32 reflectivity = 0.0f;				// Reflectivity
		size_t vertex_count;					// Vertex Count
	};

	// Load a model with Assimp
	Model LoadModel(const std::string& mPath, Texture& texture);
}

#endif // MODEL_H