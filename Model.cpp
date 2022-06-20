#include "Model.h"

using namespace Renderer;

Model::Model(const std::string& path, const MeshTextures& textures, const Material& material) : material(material)
{
	std::string newPath = Files::GetResourceDirectory() + path;

	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(newPath.c_str(), ASSIMP_FLAGS);

	LOG_INFO("Loading model: ", path, "\n");
	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		LOG_ERROR("Model Load Failed: ", importer.GetErrorString());
	}

	ProcessNode(scene->mRootNode, scene, textures);
}

void Model::ProcessNode(aiNode* node, const aiScene* scene, const MeshTextures& textures)
{
	// Iterate over all the node's meshes
	for (u32 i = 0; i < node->mNumMeshes; i++)
	{
		meshes.push_back(ProcessMesh(scene->mMeshes[node->mMeshes[i]], scene, textures));
	}

	// Iterate over all the child meshes
	for (u32 i = 0; i < node->mNumChildren; i++)
	{
		ProcessNode(node->mChildren[i], scene, textures);
	}
}

Mesh Model::ProcessMesh(aiMesh* mesh, const aiScene* scene, const MeshTextures& textures)
{
	std::vector<f32> vertices;
	std::vector<u32> indices;
	std::vector<f32> txCoords;
	std::vector<f32> normals;

	const aiVector3D aiZeroVector(0.0f, 0.0f, 0.0f);
	for (u32 i = 0; i < mesh->mNumVertices; i++)
	{
		const aiVector3D* pPos = &(mesh->mVertices[i]);
		const aiVector3D* pNormal = &(mesh->mNormals[i]);
		const aiVector3D* pTexCoord = mesh->HasTextureCoords(0) ? &(mesh->mTextureCoords[0][i]) : &aiZeroVector;

		vertices.push_back(pPos->x);
		vertices.push_back(pPos->y);
		vertices.push_back(pPos->z);

		txCoords.push_back(pTexCoord->x);
		txCoords.push_back(pTexCoord->y);

		normals.push_back(pNormal->x);
		normals.push_back(pNormal->y);
		normals.push_back(pNormal->z);
	}

	for (u32 i = 0; i < mesh->mNumFaces; ++i)
	{
		const aiFace& face = mesh->mFaces[i];
		assert(face.mNumIndices == 3);
		indices.push_back(face.mIndices[0]);
		indices.push_back(face.mIndices[1]);
		indices.push_back(face.mIndices[2]);
	}

	return Mesh(vertices, indices, txCoords, normals, ProcessMaterial(mesh, scene, textures));
}

MeshTextures Model::ProcessMaterial(aiMesh* mesh, const aiScene* scene, const MeshTextures& pTextures)
{
	aiString path;
	MeshTextures textures = pTextures;

	aiMaterial* mat = scene->mMaterials[mesh->mMaterialIndex];
	mat->GetTexture(aiTextureType_DIFFUSE, 0, &path);
	if (path.length > 0)
	{
		textures.diffuse = std::make_shared<Texture>(path.C_Str(), PathType::ABSOLUTE);
	}

	path.Clear();
	mat->GetTexture(aiTextureType_SPECULAR, 0, &path);
	if (path.length > 0)
	{
		textures.specular = std::make_shared<Texture>(path.C_Str(), PathType::ABSOLUTE);
	}

	return textures;
}