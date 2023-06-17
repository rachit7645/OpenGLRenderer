#ifndef SHARED_BUFFER_H
#define SHARED_BUFFER_H

#include "GLM.h"
#include "UniformBuffer.h"
#include "Camera.h"
#include "GL.h"

namespace Renderer
{
    class SharedBuffer : public UniformBuffer
    {
    public:
        // GLSL implementation struct
        struct ALIGN_GLSL_STD140 SharedBufferGLSL
        {
            // Clip plane
            glm::vec4 clipPlane;
            // Camera position
            glm::vec4 cameraPos;
            // Screen resolution + Near Plane + Far Plane
            glm::vec4 resolution;
        };

        // Main constructor
        SharedBuffer();

        // Loading functions
        void LoadClipPlane(const glm::vec4& clipPlane);
        void LoadCameraPos(const Entities::Camera& camera);
        void LoadResolution(const glm::ivec2& dimensions, f32 nearPlane, f32 farPlane);
    };

}

#endif
