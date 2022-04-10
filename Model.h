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
		Model(VertexArray vao, Texture& tx, const size_t vertex_count);
		VertexArray vao;						// Vertex Attribute Objects
		Texture texture;						// Texture
		f32 shineDamper = 1.0f;
		f32 reflectivity = 0.0f;
		size_t vertex_count;
	};

	Model LoadModel(const std::string& mPath, Texture& texture);
}

#endif // MODEL_H