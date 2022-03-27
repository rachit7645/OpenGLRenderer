#ifndef SDL_WINDOW_H
#define SDL_WINDOW_H

#include <vector>
#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <glm/glm.hpp>

#include "Log.h"
#include "Util.h"
#include "GLRenderer.h"
#include "StaticShader.h"
#include "Model.h"

namespace Window
{
	// Choosing a 16:9 Resolution
	constexpr s32 WINDOW_WIDTH = 1024, WINDOW_HEIGHT = 576;
	constexpr u32 WINDOW_FLAGS = SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN;

	class SDLWindow
	{
	public:
		SDLWindow();

		void MainLoop();
		bool PollEvents();
		void CalculateFPS();
		void CleanUp();

		// This is a surprise tool that will help up later
		f64 delta;

	private:
		u64 startTime, endTime;
		f64 FPS;

		SDL_Window* window = nullptr;
		SDL_GLContext glContext = nullptr;
		SDL_Event event;	
	};

}

#endif // WINDOW_H