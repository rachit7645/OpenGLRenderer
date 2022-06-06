#ifndef SKYBOX_H
#define SKYBOX_H

#include <vector>
#include <memory>

#include "VertexArray.h"
#include "CubeMap.h"

namespace Entities
{
	constexpr f32 SKYBOX_SIZE = 500.0f;

	class Skybox
	{
	public:
		Skybox();

		std::shared_ptr<Renderer::VertexArray> vao;
		std::shared_ptr<Renderer::CubeMap> cubeMap;
	};
}


#endif