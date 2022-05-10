#ifndef MODEL_H
#define MODEL_H

#include <vector>
#include <memory>

#include "VertexArray.h"
#include "Texture.h"
#include "Material.h"
#include "Log.h"
#include "Util.h"

namespace Renderer
{
	class Model
	{
	public:
		Model(const std::string& path, const Material& material, std::shared_ptr<Texture>& texture);

		Renderer::Material material;					// Material information for the model
		s32 vertexCount;								// Vertex Count

		std::shared_ptr<Renderer::VertexArray> vao;		// Vertex Attribute Object
		std::shared_ptr<Renderer::Texture> texture;		// Texture
	};
}

#endif // MODEL_H