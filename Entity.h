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
		// Usings
        using MdPtr = std::shared_ptr<Renderer::Model>;

		// Default constructor
		Entity() = default;
		// Main constructor
		Entity
        (
			MdPtr sharedModel,
			const glm::vec3& position,
			const glm::vec3& rotation,
			f32 scale
		);

		// Pointer to model
		MdPtr model;
		// Position
		glm::vec3 position;
		// Rotation
		glm::vec3 rotation;
		// Scale
		f32 scale;
	};
}

#endif