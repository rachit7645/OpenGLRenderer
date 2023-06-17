#ifndef CAMERA_H
#define CAMERA_H

#include "GLM.h"
#include "Util.h"
#include "Player.h"

namespace Entities
{
    class Camera
    {
    public:
        // Camera update function
        void Move();
        // Invert pitch
        void InvertPitch();
        // Get view matrix
        glm::mat4 GetViewMatrix() const;

        // Global flag to move camera
        static bool& GetToMoveCamera();
        // Global flag to zoom camera
        static bool& GetToZoomCamera();

        // Player
        Player* player = nullptr;
        // Position
        glm::vec3 position = {22.0f, 12.0f, -3.0f};
        // Euler rotation
        glm::vec3 rotation = {30.0f, 270.0f, 0.0f};
        // Distance from player
        f32 distance = 25.0f;
    private:
        // Display ImGui widgets
        void ImGuiDisplay();
        // Calculates zoom aka distance from player
        void CalculateZoom();
        // Calculates pitch (m_rotation.x)
        void CalculatePitch();
        // Calculates angle around player
        void CalculateAAP();
        // Calculates position and rotation
        void CalculatePosition();

        // Angle around player
        f32 m_angle = 0.0f;
        // Flag to cap pitch
        bool m_capPitch = true;
    };
}

#endif