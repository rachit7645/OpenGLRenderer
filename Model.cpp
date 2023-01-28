#include "Model.h"

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>

#include "Resources.h"
#include "Log.h"
#include "Files.h"
#include "Vertex.h"
#include "AABB.h"

// Using namespaces
using namespace Renderer;

// Usings
using Engine::Files;
using Engine::Resources;

// Assimp flags
constexpr u32 ASSIMP_FLAGS = aiProcess_Triangulate              | // Only triangles are supported
							 aiProcess_FlipUVs                  | // Flip textures
							 aiProcess_OptimizeMeshes           | // Optimise meshes
							 aiProcess_OptimizeGraph            | // Optimise scene graph
							 aiProcess_GenSmoothNormals         | // Generate normals
							 aiProcess_CalcTangentSpace         | // Generate tangents
							 aiProcess_GenUVCoords              | // Generate texture coordinates
							 aiProcess_GenBoundingBoxes         | // Generate bounding boxes
							 aiProcess_JoinIdenticalVertices    | // Join identical vertex groups
							 aiProcess_ImproveCacheLocality     | // Improves cache efficiency
							 aiProcess_RemoveRedundantMaterials ; // Removes unnecessary materials


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

	// Pre-allocate memory
	vertices.reserve(mesh->mNumVertices);
	indices.reserve(mesh->mNumFaces * 3);

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

	// Get AABB of mesh
	auto aabb = Maths::AABB(mesh->mAABB);
	// Return mesh
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
	auto GetTexture = [directory, mat] (TxPtr& texture, aiTextureType type)
	{
		// Path variable
		aiString path;
		// Resource handler
		auto& resources = Resources::GetInstance();
		// Get texture
		mat->GetTexture(type, 0, &path);
		// If texture is available
		if (path.length > 0)
		{
			texture = resources.GetTexture(directory + path.C_Str());
		}
	};

	// Albedo
	GetTexture(textures.albedo, aiTextureType_BASE_COLOR);
	// Normal
	GetTexture(textures.normal, aiTextureType_NORMALS);
	// AO + Roughness + Metallic
	GetTexture(textures.aoRghMtl, aiTextureType_DIFFUSE_ROUGHNESS);
	// Emission
	GetTexture(textures.emmisive, aiTextureType_EMISSIVE);

	return textures;
}