#ifndef ENTITY_H
#define ENTITY_H

#include <memory>
#include <glm/glm.hpp>

#include "Util.h"
#include "Model.h"

namespace Entities
{
	class Entity
	{
	public:
		Entity(std::shared_ptr<Renderer::Model> p_model, const glm::vec3 &pos, const glm::vec3 &rot, f32 sc);

		std::shared_ptr<Renderer::Model> model;
		glm::vec3 position;
		glm::vec3 rotation;
		f32 scale;
	};
}

#endif