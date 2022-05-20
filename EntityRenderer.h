#ifndef ENTITY_RENDERER_H
#define ENTITY_RENDERER_H

#include <unordered_map>
#include <memory>
#include <GL/glew.h>

#include "GLM.h"
#include "Util.h"
#include "Maths.h"
#include "Entity.h"
#include "StaticShader.h"

namespace Renderer
{
	class EntityRenderer
	{
	public:
		EntityRenderer(Shader::StaticShader &shaderRef);
		Shader::StaticShader &shader;

		// Render THE MAP
		void Render(const std::unordered_map<std::shared_ptr<Model>, std::vector<Entities::Entity>> &entities);
	private:
		// Prepare each model for render
		void PrepareModel(const std::shared_ptr<Model> &model);
		// Prepare each mesh in the model for render
		void PrepareMesh(const Mesh &mesh);		
		// Prepare each instance
		void PrepareInstance(const Entities::Entity &entity);
		// Unbind the model
		void UnbindModel(const std::shared_ptr<Model> &model);
		// Unbind the mesh
		void UnbindMesh();		
		// Enable GL_BACK culling
		void EnableCulling();
		// Disable culling
		void DisableCulling();
	};
}

#endif