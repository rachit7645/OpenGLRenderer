#include "Entity.h"

using namespace Entities;
using Renderer::Model;

Entity::Entity(std::shared_ptr<Model> &sharedModel, const glm::vec3 &position, const glm::vec3 &rotation, f32 scale)
	: model(sharedModel), position(position), rotation(rotation), scale(scale) {}