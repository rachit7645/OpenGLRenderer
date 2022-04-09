#ifndef ENTITY_H
#define ENTITY_H

#include <glm/glm.hpp>

#include "Util.h"
#include "Model.h"

namespace Entities
{
	class Entity
	{
	public:
		Entity(Renderer::Model &p_model, const glm::vec3 &pos, const glm::vec3 &rot, f32 sc);

		Renderer::Model& model;
		glm::vec3 position;
		glm::vec3 rotation;
		f32 scale;
	};
}

#endif