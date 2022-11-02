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
	class Model
	{
	public:
		Model
		(
			const std::string_view path,
			const MeshTextures& textures
		);

		std::vector<Mesh> meshes;
	private:
		void ProcessNode
		(
			aiNode* node,
			const aiScene* scene,
			const MeshTextures& textures
		);

		Mesh ProcessMesh
		(
			aiMesh* mesh,
			const aiScene* scene,
			const MeshTextures& textures
		);

		MeshTextures ProcessTextures
		(
			aiMesh* mesh,
			const aiScene* scene,
			const MeshTextures& pTextures
		);
	};
}

#endif