#include "Model.h"

// Avoid Global Namespace Corruption

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

using namespace Renderer;

Model::Model(std::shared_ptr<VertexArray> vao, std::shared_ptr<Texture> tx, size_t vertex_count)
	: vao{ vao }, texture{ tx }, vertex_count{vertex_count} {}

Model Renderer::LoadModel(const std::string &mPath, std::shared_ptr<Texture> texture)
{
	std::string newPath;
#ifdef _DEBUG
	newPath = "../" + mPath;
#else
	newPath = mPath;
#endif

	Assimp::Importer importer;
	constexpr u32 ASSIMP_FLAGS = aiProcess_Triangulate | aiProcess_FlipUVs;
	const auto *scene = importer.ReadFile(newPath.c_str(), ASSIMP_FLAGS);

	if (!scene)
	{
		Logger::LogAndExit(static_cast<std::string>("Model Load Failed: ") +
		static_cast<std::string>(importer.GetErrorString()) , ASSIMP_LOAD_FAILED);
	}

	const auto *ai_mesh = scene->mMeshes[0];
	std::vector<f32> vertices;
	std::vector<u32> indices;
	std::vector<f32> txCoords;
	std::vector<f32> normals;

	const aiVector3D aiZeroVector(0.0f, 0.0f, 0.0f);
	for (u32 i = 0; i < ai_mesh->mNumVertices; ++i)
	{
		const aiVector3D *pPos = &(ai_mesh->mVertices[i]);
		const aiVector3D *pNormal = &(ai_mesh->mNormals[i]);
		const aiVector3D *pTexCoord = ai_mesh->HasTextureCoords(0) ? &(ai_mesh->mTextureCoords[0][i]) : &aiZeroVector;

		vertices.push_back(pPos->x);
		vertices.push_back(pPos->y);
		vertices.push_back(pPos->z);

		txCoords.push_back(pTexCoord->x);
		txCoords.push_back(pTexCoord->y);

		normals.push_back(pNormal->x);
		normals.push_back(pNormal->y);
		normals.push_back(pNormal->z);
	}

	for (u32 i = 0; i < ai_mesh->mNumFaces; ++i)
	{
		const aiFace &face = ai_mesh->mFaces[i];
		assert(face.mNumIndices == 3);
		indices.push_back(face.mIndices[0]);
		indices.push_back(face.mIndices[1]);
		indices.push_back(face.mIndices[2]);
	}

	return { std::make_shared<VertexArray>(vertices, indices, txCoords, normals) , texture, indices.size()};
}