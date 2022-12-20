#include "Model.h"

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>

#include "Resources.h"
#include "Log.h"
#include "Files.h"
#include "Vertex.h"
#include "AABB.h"

// HACK: Assimp doesn't define a macro for this
#ifndef AI_MATKEY_NORMALS_TEXTURE
#define AI_MATKEY_NORMALS_TEXTURE aiTextureType_NORMALS, 0
#endif

using namespace Renderer;

using Files::FileHandler;

constexpr u32 ASSIMP_FLAGS = aiProcess_Triangulate   | aiProcess_FlipUVs          | aiProcess_OptimizeMeshes   |
							 aiProcess_OptimizeGraph | aiProcess_GenSmoothNormals | aiProcess_CalcTangentSpace |
							 aiProcess_GenUVCoords;

Model::Model(const std::string_view path, const MeshTextures& textures)
{
	Assimp::Importer importer;

	LOG_INFO("Loading model: {}\n", path);

	auto& files          = FileHandler::GetInstance();
	const aiScene* scene = importer.ReadFile((files.GetResourceDirectory() + path.data()).c_str(), ASSIMP_FLAGS);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		LOG_ERROR("Model Load Failed: {}", importer.GetErrorString());
	}

	ProcessNode(scene->mRootNode, scene, textures, files.GetDirectory(path));
}

void Model::ProcessNode
(
	aiNode* node,
	const aiScene* scene,
	const MeshTextures& textures,
	const std::string& directory
)
{
	// Iterate over all the node's meshes
	for (u32 i = 0; i < node->mNumMeshes; ++i)
	{
		meshes.emplace_back(ProcessMesh(scene->mMeshes[node->mMeshes[i]], scene, textures, directory));
	}

	// Iterate over all the child meshes
	for (u32 i = 0; i < node->mNumChildren; ++i)
	{
		ProcessNode(node->mChildren[i], scene, textures, directory);
	}
}

Mesh Model::ProcessMesh
(
	aiMesh* mesh,
	const aiScene* scene,
	const MeshTextures& textures,
	const std::string& directory
)
{
	// Data vectors
	std::vector<Vertex> vertices;
	std::vector<u32>    indices;

	for (u32 i = 0; i < mesh->mNumVertices; i++)
	{
		// Retrieve data
		const aiVector3D& position = mesh->mVertices[i];
		const aiVector3D& normal   = mesh->mNormals[i];
		const aiVector3D& texCoord = mesh->mTextureCoords[0][i];
		const aiVector3D& tangent  = mesh->mTangents[i];

		// Convert to GL friendly data
		vertices.emplace_back
		(
			glm::ai_cast(position),
			glm::ai_cast(texCoord),
			glm::ai_cast(normal),
			glm::ai_cast(tangent)
		);
	}

	for (u32 i = 0; i < mesh->mNumFaces; ++i)
	{
		const aiFace& face = mesh->mFaces[i];
		indices.emplace_back(face.mIndices[0]);
		indices.emplace_back(face.mIndices[1]);
		indices.emplace_back(face.mIndices[2]);
	}

	// Get AABB of the mesh
	auto aabb = Maths::AABB(mesh->mAABB);

	// Return
	return Mesh(vertices, indices, ProcessTextures(mesh, scene, textures, directory), aabb);
}

MeshTextures Model::ProcessTextures
(
	aiMesh* mesh,
	const aiScene* scene,
	const MeshTextures& pTextures,
	const std::string& directory
)
{
	// Default textures
	MeshTextures textures = pTextures;
	// Current materials
	aiMaterial* mat = scene->mMaterials[mesh->mMaterialIndex];

	// Utility lambda
	auto GetTexture = [directory, mat] (TxPtr& texture, aiTextureType type, unsigned int index)
	{
		// Path variable
		aiString path;
		// Get texture
		mat->GetTexture(type, index, &path);
		// If texture is available
		if (path.length > 0)
		{
			texture = Resources::GetTexture(directory + path.C_Str());
		}
	};

	// Albedo
	GetTexture(textures.albedo, AI_MATKEY_BASE_COLOR_TEXTURE);
	// Normal
	GetTexture(textures.normal, AI_MATKEY_NORMALS_TEXTURE);
	// AO + Roughness + Metallic
	GetTexture(textures.aoRghMtl, AI_MATKEY_ROUGHNESS_TEXTURE);

	return textures;
}