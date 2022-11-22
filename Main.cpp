#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>

#include "Window.h"
#include "Util.h"

// TODO: Add options parser (JSON?)

int main(UNUSED int argc, UNUSED char** argv)
{
	// Create Window and start main loop
	Window::SDLWindow().MainLoop();
	return 0;
}