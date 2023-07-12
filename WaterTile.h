#ifndef WATER_TILE_H
#define WATER_TILE_H

#include <memory>

#include "GLM.h"
#include "Texture.h"
#include "Transform.h"

namespace Waters
{
    // Tile size
    constexpr f32 WATER_TILE_SIZE = 30.0f;

    class WaterTile
    {
    public:
        // Usings
        using TxPtr = std::shared_ptr<Renderer::Texture>;

        // Default constructor
        WaterTile() = default;
        // Main constructor
        WaterTile(TxPtr dudvMap, TxPtr normalMap, const Entities::Transform& transform);

        // Maps
        TxPtr dudvMap;
        TxPtr normalMap;
        // Transform
        Entities::Transform transform;
    };
}

#endif