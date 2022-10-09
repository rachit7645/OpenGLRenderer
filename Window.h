#ifndef SDL_WINDOW_H
#define SDL_WINDOW_H

#include <vector>
#include <chrono>
#include <SDL2/SDL.h>

#include "GLM.h"
#include "Util.h"
#include "Light.h"

namespace Window
{
	// Choosing a 16:9 Resolution
	constexpr glm::ivec2 DIMENSIONS = {1024, 576};

	class SDLWindow
	{
	public:
		// Creates a SDL window, gets an opengl context, and sets up GL values
		SDLWindow();
		// Free SDL memory resources
		~SDLWindow();
		// Main loop for the engine
		void MainLoop();
	private:
		// Display ImGui Widgets
		void ImGuiDisplay(std::vector<Entities::Light>& lights);
		// Update options menu
		void ImGuiUpdate() const;
		// Function to process SDL Events
		bool PollEvents();
		// Calculates the FPS and the frame delta
		void CalculateFPS();

		using Clock     = std::chrono::steady_clock;
		using TimePoint = std::chrono::time_point<Clock>;

		TimePoint startTime;
		TimePoint frameStartTime;
		TimePoint endTime;

		f32 frameTime = 0.0f;
		f32 FPS       = 0.0f;
		f32 finalFPS  = 0.0f;

		// Debug variables
		bool wireframe       = false;
		bool vsync           = true;
		bool isInputCaptured = true;

		// SDL Resources
		SDL_Window*   window     = nullptr;
		SDL_GLContext glContext  = nullptr;
		SDL_Event     event      = {};
	};

}

#endif