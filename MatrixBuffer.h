#ifndef MATRIX_BUFFER_H
#define MATRIX_BUFFER_H

#include "GLM.h"
#include "UniformBuffer.h"
#include "Camera.h"
#include "GL.h"

namespace Renderer
{
    class MatrixBuffer : public UniformBuffer
    {
    public:
        // Internal representation of GPU data
        struct ALIGN_GLSL_STD140 MatrixBufferGLSL
        {
            // Regular matrices
            glm::mat4 projectionMatrix;
            glm::mat4 viewMatrix;
            // Inverse matrices
            glm::mat4 invProjection;
            glm::mat4 invCameraView;
        };

        // Main constructor
        MatrixBuffer();

        // Load view and inverse view matrix
        void LoadView(const Entities::Camera& camera);
        // Load projection and inverse projection matrix
        void LoadProjection(const glm::mat4& projection);
    };
}

#endif