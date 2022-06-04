#ifndef SKYBOX_H
#define SKYBOX_H

#include <vector>
#include <memory>

#include "VertexArray.h"
#include "CubeMap.h"

namespace Entities
{
	constexpr f32 SKYBOX_SIZE = 500.0f;
	const std::vector<std::string> SKYBOX_TEXTURE_FILES =
	{
		"gfx/right.png", "gfx/left.png", "gfx/top.png",
		"gfx/bottom.png", "gfx/back.png", "gfx/front.png"
	};

	class Skybox
	{
	public:
		Skybox();

		std::shared_ptr<Renderer::VertexArray> vao;
		std::shared_ptr<Renderer::CubeMap> cubeMap;
		s32 vertexCount;
	};
}


#endif