#include "Model.h"

using namespace Renderer;

Model::Model(std::vector<f32> &vert, std::vector<u32> &indi,
	std::vector<f32> &txCoords, const std::string &txPath)
	: vertices{ vert }, indices{ indi }, textureCoords{ txCoords },
	vao(vertices, indices, textureCoords), texture(txPath) {}

Model Renderer::LoadModel(const std::string& mPath, const std::string& txPath)
{
	std::string newPath;
	#ifdef _DEBUG
		newPath = "../" + mPath;
	#else
		newPath = mPath;
	#endif

	Assimp::Importer importer;
	const aiScene *scene = importer.ReadFile(newPath.c_str(), ASSIMP_FLAGS);

	if (!scene)
	{
		Logger::LogAndExit("Model Load Failed: ", ASSIMP_LOAD_FAILED);
	}

	const aiMesh *ai_model = scene->mMeshes[0];
	std::vector<f32> vertices;
	std::vector<u32> indices;
	std::vector<f32> txCoords;

	const aiVector3D aiZeroVector(0.0f, 0.0f, 0.0f);
	for (u32 i = 0; i < ai_model->mNumVertices; i++)
	{
		const aiVector3D *pPos = &(ai_model->mVertices[i]);
		UNUSED const aiVector3D *pNormal = &(ai_model->mNormals[i]);
		const aiVector3D *pTexCoord = ai_model->HasTextureCoords(0) ? &(ai_model->mTextureCoords[0][i]) : &aiZeroVector;

		vertices.push_back(pPos->x);
		vertices.push_back(pPos->y);
		vertices.push_back(pPos->z);

		txCoords.push_back(pTexCoord->x);
		txCoords.push_back(pTexCoord->y);
	}

	for (u32 i = 0; i < ai_model->mNumFaces; i++)
	{
		const aiFace &face = ai_model->mFaces[i];
		assert(face.mNumIndices == 3);
		indices.push_back(face.mIndices[0]);
		indices.push_back(face.mIndices[1]);
		indices.push_back(face.mIndices[2]);
	}

	return Model(vertices, indices, txCoords, txPath);
}