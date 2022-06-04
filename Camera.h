#ifndef CAMERA_H
#define CAMERA_H

#include <SDL2/SDL.h>

#include "GLM.h"
#include "Imgui.h"
#include "Util.h"
#include "Player.h"

// FIXME: Too many global variables

inline s32 g_MouseScrollY = 0;
inline glm::ivec2 g_MousePos;

inline bool g_ToMoveCamera = true;
inline bool g_ToZoomCamera = true;

namespace Entities
{
	constexpr auto CAMERA_SPEED = 45.0f;

	class Camera
	{
	public:
		Camera(Player& playerRef);

		Player& player;
		glm::vec3 position = { 0.0f, 0.0f, 0.0f };
		glm::vec3 rotation = { 5.0f, 0.0f, 0.0f };

		void Move();
		void ImGuiDisplay();
	private:
		bool capPitch = true;

		f32 distanceFromPlayer = 35.0f;
		f32 angleAroundPlayer = 0.0f;

		void CheckInputs();
		void CalculateZoom();
		void CalculatePitch();
		void CalculateAngleAroundPlayer();
		void CalculatePosition(f32 hDistance, f32 vDistance);
	};
}

#endif