#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>

#include "Window.h"
#include "Util.h"

int main(UNUSED int argc, UNUSED char** argv)
{
	// Create window
	auto window = Engine::Window();
	// Start main loop
	window.MainLoop();
	// Return
	return 0;
}