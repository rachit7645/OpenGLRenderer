#ifndef SDL_WINDOW_H
#define SDL_WINDOW_H

#include <chrono>
#include <SDL2/SDL.h>

#include "GLM.h"
#include "Util.h"

namespace Window
{
	// Choosing a 16:9 Resolution
	constexpr glm::ivec2 WINDOW_DIMENSIONS = {1024, 576};

	class SDLWindow
	{
	public:
		// Usings
		using Clock     = std::chrono::steady_clock;
		using TimePoint = std::chrono::time_point<Clock>;
		// Creates a SDL window, gets an opengl context, and sets up GL values
		SDLWindow();
		// Free SDL memory resources
		~SDLWindow();
		// Main loop for the engine
		void MainLoop();
	private:
		// Display ImGui Widgets
		void ImGuiDisplay();
		// Update options menu
		void ImGuiUpdate();
		// Function to process SDL Events
		bool PollEvents();
		// Calculates the FPS and the frame delta
		void CalculateFPS();

		// Time variables
		TimePoint startTime;
		TimePoint frameStartTime;
		TimePoint endTime;

		// FPS counters
		f32 frameTime = 0.0f;
		f32 FPS       = 0.0f;
		f32 finalFPS  = 0.0f;

		// Debug variables
		bool wireframe       = false;
		bool vsync           = true;
		bool isInputCaptured = true;

		// State
		s32    currentLight    = 0;
		GLenum currentPolyMode = GL_FILL;
		s32    currentSwapMode = -1;

		// SDL Resources
		SDL_Window*   window     = nullptr;
		SDL_GLContext glContext  = nullptr;
		SDL_Event     event      = {};
	};

}

#endif