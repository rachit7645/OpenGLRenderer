#ifndef CAMERA_H
#define CAMERA_H

#include <SDL2/SDL.h>

#include "GLM.h"
#include "Imgui.h"
#include "Util.h"
#include "Player.h"
#include "Inputs.h"

// HACK: Global Flags
inline bool g_ToMoveCamera = true;
inline bool g_ToZoomCamera = true;

namespace Entities
{
	constexpr auto CAMERA_SPEED = 45.0f;
	constexpr auto CAMERA_ZOOM_SPEED = 1.0f;

	class Camera
	{
	public:
		Camera(Player& playerRef);

		Player& player;
		glm::vec3 position = { 0.0f, 0.0f, 0.0f };
		glm::vec3 rotation = { 5.0f, 0.0f, 0.0f };

		void Move();
	private:
		const glm::ivec2& mousePos;
		const glm::ivec2& mouseScroll;

		bool capPitch = true;

		f32 distanceFromPlayer = 35.0f;
		f32 angleAroundPlayer = 0.0f;

		void ImGuiDisplay();
		void CheckInputs();
		void CalculateZoom();
		void CalculatePitch();
		void CalculateAngleAroundPlayer();
		void CalculatePosition(f32 hDistance, f32 vDistance);
	};
}

#endif