#include "Model.h"

using namespace Renderer;

Model::Model(std::vector<f32>& vert, std::vector<u32>& indi,
			 std::vector<f32>& txCoords, const std::string& txPath)
			 : vertices{ vert }, indices{ indi }, textureCoords{ txCoords },
			   vao(vertices, indices, textureCoords), texture(txPath) {}