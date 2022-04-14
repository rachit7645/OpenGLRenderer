#include "Entity.h"

using namespace Entities;
using namespace Renderer;

Entity::Entity(std::shared_ptr<Model> p_model, const glm::vec3 &pos, const glm::vec3 &rot, f32 sc)
	: model{ p_model }, position{ pos }, rotation{ rot }, scale{ sc } {}