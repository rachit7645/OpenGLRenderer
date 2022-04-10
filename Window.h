#ifndef SDL_WINDOW_H
#define SDL_WINDOW_H

#include <vector>
#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <glm/glm.hpp>

#include "Log.h"
#include "Util.h"
#include "EntityRenderer.h"
#include "StaticShader.h"
#include "Model.h"
#include "Entity.h"
#include "Camera.h"
#include "Light.h"
#include "MasterRenderer.h"

inline const u8* g_Keys;
inline f64 g_Delta = 1.0f;

namespace Window
{
	// Choosing a 16:9 Resolution
	constexpr glm::vec2 DIMENSIONS = { 1024, 576 };
	constexpr u32 WINDOW_FLAGS = SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN;

	class SDLWindow
	{
	public:
		SDLWindow();
		~SDLWindow();

		void MainLoop();
		bool PollEvents();
		void CalculateFPS();

	private:
		inline void CreateProjectionMatrix();
		void InitGL();

		u64 startTime, endTime;
		f64 FPS;

		#ifdef _DEBUG
			bool wireframe = false;
			bool vsync = true;
		#endif

		SDL_Window* window = nullptr;
		SDL_GLContext glContext = nullptr;
		SDL_Event event;	
	};

}

#endif // WINDOW_H