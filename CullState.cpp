#include "CullState.h"

// Using namespaces
using namespace Entities;

CullState::CullState(const std::vector<Renderer::Mesh>& meshes)
    : isEntityCulled(meshes.size())
{
    // For each mesh
    for (const auto& mesh : meshes)
    {
        // Copy ID
        isEntityCulled.emplace(std::pair<usize, bool>(mesh.id, false));
    }
}