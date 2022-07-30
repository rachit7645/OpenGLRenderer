#ifndef SDL_WINDOW_H
#define SDL_WINDOW_H

#include <chrono>
#include <SDL2/SDL.h>

#include "GLM.h"
#include "Util.h"
#include "Light.h"

namespace Window
{
	// Choosing a 16:9 Resolution
	constexpr glm::ivec2 DIMENSIONS = { 1024, 576 };
	constexpr u32 SDL_WINDOW_FLAGS  = SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN;

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
		void ImGuiDisplay();
		// Update options menu
		void ImGuiUpdate() const;
		// Function to process SDL Events
		bool PollEvents();
		// Calculates the FPS and the frame delta
		void CalculateFPS();
		// Set OpenGL stuff
		void InitGL();

		// FPS Counting Variables
		std::chrono::time_point<std::chrono::steady_clock> startTime, frameStartTime, endTime;
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