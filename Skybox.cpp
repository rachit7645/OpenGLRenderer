#include "Skybox.h"

using namespace Entities;

using Renderer::VertexArray;
using Renderer::CubeMap;

const std::vector<f32> SKYBOX_VERTICES =
{
	-SKYBOX_SIZE, SKYBOX_SIZE, -SKYBOX_SIZE,
	-SKYBOX_SIZE, -SKYBOX_SIZE, -SKYBOX_SIZE,
	SKYBOX_SIZE, -SKYBOX_SIZE, -SKYBOX_SIZE,
	SKYBOX_SIZE, -SKYBOX_SIZE, -SKYBOX_SIZE,
	SKYBOX_SIZE, SKYBOX_SIZE, -SKYBOX_SIZE,
	-SKYBOX_SIZE, SKYBOX_SIZE, -SKYBOX_SIZE,

	-SKYBOX_SIZE, -SKYBOX_SIZE, SKYBOX_SIZE,
	-SKYBOX_SIZE, -SKYBOX_SIZE, -SKYBOX_SIZE,
	-SKYBOX_SIZE, SKYBOX_SIZE, -SKYBOX_SIZE,
	-SKYBOX_SIZE, SKYBOX_SIZE, -SKYBOX_SIZE,
	-SKYBOX_SIZE, SKYBOX_SIZE, SKYBOX_SIZE,
	-SKYBOX_SIZE, -SKYBOX_SIZE, SKYBOX_SIZE,

	SKYBOX_SIZE, -SKYBOX_SIZE, -SKYBOX_SIZE,
	SKYBOX_SIZE, -SKYBOX_SIZE, SKYBOX_SIZE,
	SKYBOX_SIZE, SKYBOX_SIZE, SKYBOX_SIZE,
	SKYBOX_SIZE, SKYBOX_SIZE, SKYBOX_SIZE,
	SKYBOX_SIZE, SKYBOX_SIZE, -SKYBOX_SIZE,
	SKYBOX_SIZE, -SKYBOX_SIZE, -SKYBOX_SIZE,

	-SKYBOX_SIZE, -SKYBOX_SIZE, SKYBOX_SIZE,
	-SKYBOX_SIZE, SKYBOX_SIZE, SKYBOX_SIZE,
	SKYBOX_SIZE, SKYBOX_SIZE, SKYBOX_SIZE,
	SKYBOX_SIZE, SKYBOX_SIZE, SKYBOX_SIZE,
	SKYBOX_SIZE, -SKYBOX_SIZE, SKYBOX_SIZE,
	-SKYBOX_SIZE, -SKYBOX_SIZE, SKYBOX_SIZE,

	-SKYBOX_SIZE, SKYBOX_SIZE, -SKYBOX_SIZE,
	SKYBOX_SIZE, SKYBOX_SIZE, -SKYBOX_SIZE,
	SKYBOX_SIZE, SKYBOX_SIZE, SKYBOX_SIZE,
	SKYBOX_SIZE, SKYBOX_SIZE, SKYBOX_SIZE,
	-SKYBOX_SIZE, SKYBOX_SIZE, SKYBOX_SIZE,
	-SKYBOX_SIZE, SKYBOX_SIZE, -SKYBOX_SIZE,

	-SKYBOX_SIZE, -SKYBOX_SIZE, -SKYBOX_SIZE,
	-SKYBOX_SIZE, -SKYBOX_SIZE, SKYBOX_SIZE,
	SKYBOX_SIZE, -SKYBOX_SIZE, -SKYBOX_SIZE,
	SKYBOX_SIZE, -SKYBOX_SIZE, -SKYBOX_SIZE,
	-SKYBOX_SIZE, -SKYBOX_SIZE, SKYBOX_SIZE,
	SKYBOX_SIZE, -SKYBOX_SIZE, SKYBOX_SIZE
};

Skybox::Skybox() : vao(std::make_shared<VertexArray>(SKYBOX_VERTICES)), cubeMap(std::make_shared<CubeMap>(SKYBOX_TEXTURE_FILES)),
vertexCount(static_cast<s32>(SKYBOX_VERTICES.size() / 3)) {}