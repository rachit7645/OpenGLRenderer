#ifndef SDL_WINDOW_H
#define SDL_WINDOW_H

#include <vector>
#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <glm/glm.hpp>

#include "Window.h"
#include "Log.h"
#include "Util.h"
#include "GLRenderer.h"
#include "StaticShader.h"

namespace Window
{
	// TODO: Make a config system

	// Choosing a 16:9 Resolution
	constexpr s32 WINDOW_WIDTH = 1024;
	constexpr s32 WINDOW_HEIGHT = 576;
	constexpr s32 WINDOW_FLAGS = SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN;

	static f64 delta;

	void CreateWindow();
	void MainLoop();
	bool PollEvents();
	void CalculateFPS();
	void CleanUp();

}

#endif