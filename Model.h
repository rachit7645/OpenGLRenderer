#ifndef MODEL_H
#define MODEL_H

#include <vector>
#include <string>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Material.h"
#include "Util.h"
#include "Mesh.h"
#include "MeshTextures.h"

namespace Renderer
{
	constexpr u32 ASSIMP_FLAGS = aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_OptimizeMeshes;

	class Model
	{
	public:
		Model(const std::string& path, const MeshTextures& textures, const Material& material = Material());

		std::vector<Mesh> meshes;
		Material          material;
	private:
		void ProcessNode(aiNode* node, const aiScene* scene, const MeshTextures& textures);
		Mesh ProcessMesh(aiMesh* mesh, const aiScene* scene, const MeshTextures& textures);
		MeshTextures ProcessMaterial(aiMesh* mesh, const aiScene* scene, const MeshTextures& pTextures);
	};
}

#endif // MODEL_H