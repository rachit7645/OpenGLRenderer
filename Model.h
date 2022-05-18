#ifndef MODEL_H
#define MODEL_H

#include <vector>
#include <memory>
#include <string>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Material.h"
#include "Log.h"
#include "Util.h"
#include "Mesh.h"

namespace Renderer
{
	class Model
	{
	public:
		Model(const std::string &path, std::shared_ptr<Texture> &texture, const Material &material = Material());

		std::string directory;
		std::vector<Mesh> meshes;
		Material material;
	private:
		void ProcessNode(aiNode *node, const aiScene *scene, std::shared_ptr<Texture> &texture);
		Mesh ProcessMesh(aiMesh *mesh, const aiScene *scene, std::shared_ptr<Texture> &texture); 
	};
}

#endif // MODEL_H