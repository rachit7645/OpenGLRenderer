#include "Entity.h"

using namespace Entities;

using Renderer::Model;

Entity::Entity
(
	MdPtr sharedModel,
	const glm::vec3& position,
	const glm::vec3& rotation,
	f32 scale
)
	: model(std::move(sharedModel)),
	  position(position),
	  rotation(rotation),
	  scale(scale)
{
}