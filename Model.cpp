#include "Model.h"

#include "Resources.h"
#include "Log.h"
#include "Files.h"
#include "Vertex.h"

using namespace Renderer;

Model::Model
(
	const std::string_view path,
	const MeshTextures& textures,
	const Material& material
)
{
	Assimp::Importer importer;

	LOG_INFO("Loading model: {}\n", path);
	const aiScene* scene = importer.ReadFile((Files::GetResourceDirectory() + path.data()).c_str(), ASSIMP_FLAGS);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		LOG_ERROR("Model Load Failed: {}", importer.GetErrorString());
	}

	ProcessNode(scene->mRootNode, scene, textures, material);
}

void Model::ProcessNode
(
	aiNode* node,
	const aiScene* scene,
	const MeshTextures& textures,
	const Material& material
)
{
	// Iterate over all the node's meshes
	for (u32 i = 0; i < node->mNumMeshes; ++i)
	{
		meshes.emplace_back(ProcessMesh(scene->mMeshes[node->mMeshes[i]], scene, textures, material));
	}

	// Iterate over all the child meshes
	for (u32 i = 0; i < node->mNumChildren; ++i)
	{
		ProcessNode(node->mChildren[i], scene, textures, material);
	}
}

Mesh Model::ProcessMesh
(
	aiMesh* mesh,
	const aiScene* scene,
	const MeshTextures& textures,
	const Material& material
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
		ProcessTextures(mesh, scene, textures),
		ProcessMaterial(mesh, scene, material)
	);
}

MeshTextures Model::ProcessTextures
(
	aiMesh* mesh,
	const aiScene* scene,
	const MeshTextures& pTextures
)
{
	aiString path;
	MeshTextures textures = pTextures;

	aiMaterial* mat = scene->mMaterials[mesh->mMaterialIndex];
	mat->GetTexture(aiTextureType_DIFFUSE, 0, &path);
	if (path.length > 0)
	{
		textures.diffuse = Resources::GetTexture(path.C_Str());
	}

	path.Clear();
	mat->GetTexture(aiTextureType_SPECULAR, 0, &path);
	if (path.length > 0)
	{
		textures.specular = Resources::GetTexture(path.C_Str());
	}

	return textures;
}

Material Model::ProcessMaterial
(
	aiMesh* mesh,
	const aiScene* scene,
	const Material& pMaterial
)
{
	Material    material = pMaterial;
	aiMaterial* mat      = scene->mMaterials[mesh->mMaterialIndex];

	mat->Get(AI_MATKEY_SHININESS,          &material.shineDamper,  nullptr);
	mat->Get(AI_MATKEY_SHININESS_STRENGTH, &material.reflectivity, nullptr);

	material.shineDamper  = std::max(material.shineDamper,  1.0f);
	material.reflectivity = std::max(material.reflectivity, 0.0f);

	return material;
}
