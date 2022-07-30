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
	constexpr auto CAMERA_ZOOM_SPEED = 1.0f;
	constexpr auto CAMERA_PITCH_MIN  = 0.0f;
	constexpr auto CAMERA_PITCH_MAX  = 90.0f;

	class Camera
	{
	public:
		explicit Camera(Player* player);

		Player*   player;
		glm::vec3 position = { 0.0f, 0.0f, 0.0f };
		glm::vec3 rotation = { 5.0f, 0.0f, 0.0f };

		// Camera update function
		void Move();
	private:
		glm::ivec2* mousePos;
		glm::ivec2* mouseScroll;

		f32 distanceFromPlayer = 35.0f;
		f32 angleAroundPlayer  = 0.0f;

		bool capPitch = true;

		// Display ImGui widgets
		void ImGuiDisplay();
		// Calculates zoom aka distance from player
		void CalculateZoom();
		// Calculates pitch (rotation.x)
		void CalculatePitch();
		// Calculates angle around player
		void CalculateAAP();
		// Calculates position, and rotation 
		void CalculatePosition();
	};
}

#endif