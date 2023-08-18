#ifndef CULL_STATE_H
#define CULL_STATE_H

#include <unordered_map>

#include "Util.h"
#include "Mesh.h"

namespace Entities
{
    struct CullState
    {
    public:
        // Default constructor
        CullState() = default;
        // Main constructor
        explicit CullState(const std::vector<Renderer::Mesh>& meshes);
        // Entity culling data
        std::unordered_map<usize, bool> isEntityCulled = {};
    };
}

#endif
