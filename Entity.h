#ifndef ENTITY_H
#define ENTITY_H

#include <memory>

#include "GLM.h"
#include "Model.h"

namespace Entities
{
	class Entity
	{
	public:
		Entity
		(
			std::shared_ptr<Renderer::Model>& sharedModel,
			const glm::vec3& position,
			const glm::vec3& rotation,
			f32 scale
		);

		std::shared_ptr<Renderer::Model> model;
		glm::vec3 position;
		glm::vec3 rotation;
		f32 	  scale;
	};
}

#endif