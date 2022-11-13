#ifndef CAMERA_H
#define CAMERA_H

#include "GLM.h"
#include "Util.h"
#include "Player.h"

// HACK: Global Flags
inline bool g_ToMoveCamera = true;
inline bool g_ToZoomCamera = true;

namespace Entities
{
	class Camera
	{
	public:
		explicit Camera(Player* player);

		Player*   player   = nullptr;
		glm::vec3 position = {0.0f, 0.0f, 0.0f};
		glm::vec3 rotation = {5.0f, 0.0f, 0.0f};
		f32       distance = 25.0f;

		// Camera update function
		void Move();
		// Invert pitch
		void InvertPitch();
	private:
		f32  m_angle    = 0.0f;
		bool m_capPitch = true;

		// Display ImGui widgets
		void ImGuiDisplay();
		// Calculates zoom aka distance from player
		void CalculateZoom();
		// Calculates pitch (m_rotation.x)
		void CalculatePitch();
		// Calculates angle around player
		void CalculateAAP();
		// Calculates position, and m_rotation
		void CalculatePosition();
	};
}

#endif