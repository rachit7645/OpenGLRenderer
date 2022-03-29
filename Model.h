#ifndef MODEL_H
#define MODEL_H

#include <vector>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "VertexArray.h"
#include "Texture.h"
#include "Log.h"
#include "Util.h"

namespace Renderer
{
	struct Model
	{
		Model(std::vector<f32>& vert, std::vector<u32>& indi, std::vector<f32>& txCoords, const std::string& txPath);
		std::vector<f32> vertices;				 // Vertices
		std::vector<u32> indices;				 // Indices
		std::vector<f32> textureCoords;			 // Texture Coordinates
		VertexArray vao;
		Texture texture;						 // Texture
	};

	constexpr u32 ASSIMP_FLAGS = aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs;
	Model LoadModel(const std::string& mPath, const std::string& txPath);
}

#endif // MODEL_H