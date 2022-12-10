#ifndef SDL_WINDOW_H
#define SDL_WINDOW_H

#include <vector>
#include <chrono>
#include <SDL2/SDL.h>

#include "GLM.h"
#include "Util.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"

namespace Engine
{
	class Window
	{
	public:
		// Creates a SDL handle, gets an opengl context, and sets up GL values
		Window();
		// Free SDL memory resources
		~Window();
		// Function to process SDL Events
		bool PollEvents();
		// SDL window handle
		SDL_Window* handle = nullptr;
	private:
		// Input capture flag
		bool m_isInputCaptured    = true;
		// OpenGL context
		SDL_GLContext m_glContext = nullptr;
		// SDL event
		SDL_Event m_event = {};
	};
}

#endif