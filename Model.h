#ifndef MODEL_H
#define MODEL_H

#include <vector>
#include <memory>

#include "VertexArray.h"
#include "Texture.h"
#include "Log.h"
#include "Util.h"

namespace Renderer
{
	struct Model
	{
		Model(std::shared_ptr<VertexArray> vao, std::shared_ptr<Renderer::Texture> tx, size_t vertex_count);
		std::shared_ptr<Renderer::VertexArray> vao;	// Vertex Attribute Object
		std::shared_ptr<Renderer::Texture> texture;	// Texture
		f32 shineDamper = 1.0f;						// Shine Dampner
		f32 reflectivity = 0.0f;					// Reflectivity
		size_t vertex_count;						// Vertex Count
	};

	// Load a model with Assimp
	Model LoadModel(const std::string& mPath, std::shared_ptr<Renderer::Texture> texture);
}

#endif // MODEL_H