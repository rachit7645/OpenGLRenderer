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
	class Model
	{
	public:
		Model(const std::string& path, std::shared_ptr<Texture> texture);

		std::shared_ptr<Renderer::VertexArray> vao;	// Vertex Attribute Object
		std::shared_ptr<Renderer::Texture> texture;	// Texture

		f32 shineDamper = 1.0f;						// Shine Dampner
		f32 reflectivity = 0.0f;					// Reflectivity
		s32 vertexCount;							// Vertex Count

		bool isTransparent = false;					// Is Transparent or not
		bool useFakeLighting = false;				// Use fake lighting or not
	};
}

#endif // MODEL_H