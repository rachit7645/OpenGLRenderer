#ifndef MODEL_H
#define MODEL_H

#include <vector>
#include <string_view>

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
		Model
		(
			const std::string_view path,
			const MeshTextures& textures,
			const Material& material = Material()
		);

		std::vector<Mesh> meshes;
	private:
		void ProcessNode
		(
			aiNode* node,
			const aiScene* scene,
			const MeshTextures& textures,
			const Material& material
		);

		Mesh ProcessMesh
		(
			aiMesh* mesh,
			const aiScene* scene,
			const MeshTextures& textures,
			const Material& material
		);

		MeshTextures ProcessTextures
		(
			aiMesh* mesh,
			const aiScene* scene,
			const MeshTextures& pTextures
		);

		Material ProcessMaterial
		(
			aiMesh* mesh,
			const aiScene* scene,
			const Material& pMaterial
		);
	};
}

#endif // MODEL_H