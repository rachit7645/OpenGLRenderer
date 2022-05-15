#ifndef SDL_WINDOW_H
#define SDL_WINDOW_H

#include <SDL2/SDL.h>
#include <GL/glew.h>

#include "GLM.h"
#include "Log.h"
#include "Util.h"

// Handy global variables
inline const u8* g_Keys;
inline f32 g_Delta = 1.0f;

namespace Window
{
	// Choosing a 16:9 Resolution
	constexpr glm::vec2 DIMENSIONS = { 1024, 576 };
	constexpr u32 SDL_WINDOW_FLAGS = SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN;

	class SDLWindow
	{
	public:
		// Creates a SDL window, gets an opengl context, and sets up GL values
		SDLWindow();
		// Free SDL memory resources
		~SDLWindow();

		// Main loop for the engine
		void MainLoop();
		// Function to process SDL Events
		bool PollEvents();
		// Calculates the FPS and the the frame delta
		void CalculateFPS();

	private:
		// Set OpengGL stuff 
		void InitGL();

		// FPS Counting Variables
		u64 startTime = 0;
		u64 frameStartTime = 0;
		u64 endTime = 0;
		f32 FPS = 0.0f;

		// Debug variables
		#ifdef _DEBUG
			bool wireframe = false;
			bool vsync = true;
			bool fullscreen = false;
		#endif

		// SDL Resources
		SDL_Window* window = nullptr;
		SDL_GLContext glContext = nullptr;
		SDL_Event event;
	};

}

#endif // WINDOW_H