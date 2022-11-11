#ifndef SKYBOX_H
#define SKYBOX_H

#include <memory>

#include "VertexArray.h"
#include "CubeMap.h"

namespace Entities
{
	constexpr f32 SKYBOX_SIZE = 500.0f;

	class Skybox
	{
	public:
		// Usings
		using VAO        = std::shared_ptr<Renderer::VertexArray>;
		using CubeMapPtr = std::shared_ptr<Renderer::CubeMap>;
		// Main constructor
		Skybox();

		// Data
		VAO        vao;
		CubeMapPtr cubeMap;
	};
}


#endif