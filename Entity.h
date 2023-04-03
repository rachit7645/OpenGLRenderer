#ifndef ENTITY_H
#define ENTITY_H

#include <memory>

#include "GLM.h"
#include "Model.h"
#include "Transform.h"

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
		Entity(MdPtr sharedModel, const Transform& transform);
		// Destructor
		virtual ~Entity() = default;

		// Pointer to model
		MdPtr model = nullptr;
		// Transform
        Transform transform = {};
	};
}

#endif