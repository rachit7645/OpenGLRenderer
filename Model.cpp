#include "Model.h"

using namespace Renderer;

constexpr u32 ASSIMP_FLAGS = aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_OptimizeMeshes;

Model::Model(const std::string &path, std::shared_ptr<Texture> &texture, const Material &material) : material{ material }
{
	std::string newPath;
#ifdef _DEBUG
	newPath = "../" + path;
#else
	newPath = path;
#endif

	Assimp::Importer importer;
	const aiScene *scene = importer.ReadFile(newPath.c_str(), ASSIMP_FLAGS);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		Logger::LogAndExit(static_cast<std::string>("Model Load Failed: ") +
			static_cast<std::string>(importer.GetErrorString()), ASSIMP_LOAD_FAILED);
	}

	directory = newPath.substr(0, path.find_last_of('/'));

	ProcessNode(scene->mRootNode, scene, texture);
}

void Model::ProcessNode(aiNode *node, const aiScene *scene, std::shared_ptr<Texture> &texture)
{
	// Iterate over all the node's meshes
	for (u32 i = 0; i < node->mNumMeshes; i++)
	{
		meshes.push_back(ProcessMesh(scene->mMeshes[node->mMeshes[i]], scene, texture));
	}

	// Iterate over all the child meshes
	for (u32 i = 0; i < node->mNumChildren; i++)
	{
		ProcessNode(node->mChildren[i], scene, texture);
	}
}

Mesh Model::ProcessMesh(aiMesh *mesh, const aiScene *scene, std::shared_ptr<Texture> &texture)
{
	std::vector<f32> vertices;
	std::vector<u32> indices;
	std::vector<f32> txCoords;
	std::vector<f32> normals;

	const aiVector3D aiZeroVector(0.0f, 0.0f, 0.0f);
	for (u32 i = 0; i < mesh->mNumVertices; i++)
	{
		const aiVector3D *pPos = &(mesh->mVertices[i]);
		const aiVector3D *pNormal = &(mesh->mNormals[i]);
		const aiVector3D *pTexCoord = mesh->HasTextureCoords(0) ? &(mesh->mTextureCoords[0][i]) : &aiZeroVector;

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
		const aiFace &face = mesh->mFaces[i];
		assert(face.mNumIndices == 3);
		indices.push_back(face.mIndices[0]);
		indices.push_back(face.mIndices[1]);
		indices.push_back(face.mIndices[2]);
	}

	aiMaterial *mat = scene->mMaterials[mesh->mMaterialIndex];
	aiString path;
	mat->GetTexture(aiTextureType_DIFFUSE, 0, &path);
	if (path.length > 0)
		texture = std::make_shared<Texture>(path.C_Str(), PathType::ABSOLUTE);

	return Mesh(vertices, indices, txCoords, normals, texture);
}