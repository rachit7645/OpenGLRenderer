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
        using MdPtr = std::shared_ptr<Renderer::Model>;
		Entity
        (
			MdPtr sharedModel,
			const glm::vec3& position,
			const glm::vec3& rotation,
			f32 scale
		);

		MdPtr     model;
		glm::vec3 position;
		glm::vec3 rotation;
		f32       scale;
	};
}

#endif