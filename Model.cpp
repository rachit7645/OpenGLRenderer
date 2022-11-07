#include "Model.h"

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>

#include "Resources.h"
#include "Log.h"
#include "Files.h"
#include "Vertex.h"

// HACK: Assimp doesn't define macros for these

#ifndef AI_MATKEY_NORMALS_TEXTURE
#define AI_MATKEY_NORMALS_TEXTURE aiTextureType_NORMALS, 0
#endif

#ifndef AI_MATKEY_AO_TEXTURE
#define AI_MATKEY_AO_TEXTURE aiTextureType_AMBIENT_OCCLUSION, 0
#endif

using namespace Renderer;

constexpr u32 ASSIMP_FLAGS = aiProcess_Triangulate    |
							 aiProcess_FlipUVs        |
							 aiProcess_OptimizeMeshes |
							 aiProcess_OptimizeGraph;

Model::Model(const std::string_view path, const MeshTextures& textures)
{
	Assimp::Importer importer;

	LOG_INFO("Loading model: {}\n", path);
	const aiScene* scene = importer.ReadFile((Files::GetResourceDirectory() + path.data()).c_str(), ASSIMP_FLAGS);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		LOG_ERROR("Model Load Failed: {}", importer.GetErrorString());
	}

	ProcessNode(scene->mRootNode, scene, textures, Files::GetDirectory(path));
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
	std::vector<Vertex> vertices;
	std::vector<u32>    indices;

	for (u32 i = 0; i < mesh->mNumVertices; i++)
	{
		const auto aiZeroVector = aiVector3D(0.0f, 0.0f, 0.0f);

		const aiVector3D& position = mesh->mVertices[i];
		const aiVector3D& normal   = mesh->mNormals[i];
		const aiVector3D& texCoord = mesh->HasTextureCoords(0) ? mesh->mTextureCoords[0][i] : aiZeroVector;

		vertices.emplace_back
		(
			glm::vec3(position.x, position.y, position.z),
			glm::vec2(texCoord.x, texCoord.y),
			glm::vec3(normal.x, normal.y, normal.z)
		);
	}

	for (u32 i = 0; i < mesh->mNumFaces; ++i)
	{
		const aiFace& face = mesh->mFaces[i];
		indices.emplace_back(face.mIndices[0]);
		indices.emplace_back(face.mIndices[1]);
		indices.emplace_back(face.mIndices[2]);
	}

	return Mesh
	(
		vertices,
		indices,
		ProcessTextures(mesh, scene, textures, directory)
	);
}

MeshTextures Model::ProcessTextures
(
	aiMesh* mesh,
	const aiScene* scene,
	const MeshTextures& pTextures,
	const std::string& directory
)
{
	aiString path;
	MeshTextures textures = pTextures;

	aiMaterial* mat = scene->mMaterials[mesh->mMaterialIndex];

	// Albedo
	mat->GetTexture(AI_MATKEY_BASE_COLOR_TEXTURE, &path);
	if (path.length > 0)
	{
		textures.albedo = Resources::GetTexture(directory + path.C_Str());
	}

	// Normal
	path.Clear();
	mat->GetTexture(AI_MATKEY_NORMALS_TEXTURE, &path);
	if (path.length > 0)
	{
		textures.normal = Resources::GetTexture(directory + path.C_Str());
	}

	// Metallic + Roughness (GLTF is weird)
	path.Clear();
	mat->GetTexture(AI_MATKEY_ROUGHNESS_TEXTURE, &path);
	if (path.length > 0)
	{
		textures.mtlRgh = Resources::GetTexture(directory + path.C_Str());
	}

	// Ambient Occlusion
	path.Clear();
	mat->GetTexture(AI_MATKEY_AO_TEXTURE, &path);
	if (path.length > 0)
	{
		textures.ao = Resources::GetTexture(directory + path.C_Str());
	}

	return textures;
}