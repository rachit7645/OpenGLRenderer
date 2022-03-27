#include "Entity.h"

using namespace Entities;

Entity::Entity(const Renderer::Model &p_model, const glm::vec3 &pos, const glm::vec3 &rot, f32 sc)
	: model{ p_model }, position{ pos }, rotation{ rot }, scale{ sc } {}