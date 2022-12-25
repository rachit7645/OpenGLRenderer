#include "Model.h"

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>

#include "Resources.h"
#include "Log.h"
#include "Files.h"
#include "Vertex.h"

// HACK: Assimp doesn't define a macro for this
#ifndef AI_MATKEY_NORMALS_TEXTURE
#define AI_MATKEY_NORMALS_TEXTURE aiTextureType_NORMALS, 0
#endif

// Using namespaces
using namespace Renderer;

// Usings
using Engine::Files;
using Engine::Resources;

// Assimp flags
constexpr u32 ASSIMP_FLAGS = aiProcess_Triangulate      | // Triangles are easier to work with
							 aiProcess_FlipUVs          | // My textures are flipped
							 aiProcess_OptimizeMeshes   | // Mesh optimisation
							 aiProcess_OptimizeGraph    | // Scene optimisation
							 aiProcess_GenSmoothNormals | // Generate normals if none are available
							 aiProcess_CalcTangentSpace | // Generate tangents
							 aiProcess_GenUVCoords;       // Generate texture coordinates

Model::Model(const std::string_view path, const MeshTextures& textures)
{
	// Log
	LOG_INFO("Loading model: {}\n", path);

	// Get importer
	Assimp::Importer importer;
	// Get files handler
	auto& files = Files::GetInstance();

	// Load scene
	const aiScene* scene = importer.ReadFile((files.GetResources() + path.data()).c_str(), ASSIMP_FLAGS);
	// Check if scene is OK
	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		// Log error
		LOG_ERROR("Model Load Failed: {}", importer.GetErrorString());
	}

	// Process scene nodes
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
		// Add meshes
		meshes.emplace_back(ProcessMesh(scene->mMeshes[node->mMeshes[i]], scene, textures, directory));
	}

	// Iterate over all the child nodes
	for (u32 i = 0; i < node->mNumChildren; ++i)
	{
		// Process nodes recursively
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
	// Vertex data (packed)
	std::vector<Vertex> vertices;
	// Index data
	std::vector<u32> indices;

	// For all vertices
	for (u32 i = 0; i < mesh->mNumVertices; ++i)
	{
		// Convert to GL friendly data
		vertices.emplace_back
		(
			glm::ai_cast(mesh->mVertices[i]),
			glm::ai_cast(mesh->mTextureCoords[0][i]),
			glm::ai_cast(mesh->mNormals[i]),
			glm::ai_cast(mesh->mTangents[i])
		);
	}

	// For all faces
	for (u32 i = 0; i < mesh->mNumFaces; ++i)
	{
		// Get faces
		const aiFace& face = mesh->mFaces[i];
		// Store indices
		indices.emplace_back(face.mIndices[0]);
		indices.emplace_back(face.mIndices[1]);
		indices.emplace_back(face.mIndices[2]);
	}

	// Return mesh
	return Mesh(vertices, indices, ProcessTextures(mesh, scene, textures, directory));
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
		// Resource handler
		auto& resources = Resources::GetInstance();
		// Get texture
		mat->GetTexture(type, index, &path);
		// If texture is available
		if (path.length > 0)
		{
			texture = resources.GetTexture(directory + path.C_Str());
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