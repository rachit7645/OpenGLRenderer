#include "Window.h"

using namespace Window;

// Creates a SDL window, gets an opengl context, and sets up GL values
SDLWindow::SDLWindow()
{
	Logger::Log("Initializing SDL2\n", Logger::INFO);
	if (SDL_Init(SDL_INIT_VIDEO) != 0)
	{
		Logger::LogAndExit_SDL("SDL_Init Failed\n", SDL_INIT_FAILED);
	}

	Logger::Log("Setting up OpenGL context\n", Logger::INFO);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	// OpenGL Version 3.3 Core
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);

	// RGBA8888 Framebuffer
	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);

	// 4x MSAA Anti-Aliasing
	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 4);

	Logger::Log("Creating a window\n", Logger::INFO);
	window = SDL_CreateWindow
	(
		"Rachit's Engine", SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED, DIMENSIONS.x, DIMENSIONS.y, WINDOW_FLAGS
	);
	if (window == nullptr)
	{
		Logger::LogAndExit_SDL("SDL_CreateWindow Failed\n", SDL_CREATE_WINDOW_FAILED);
	}
	// For sanity, raise window
	SDL_RaiseWindow(window);

	// Basically useless GLContext
	// You don't get more than GL 1.1 for compatibilty reasons (Windows YOU SUCK)
	glContext = SDL_GL_CreateContext(window);
	if (glContext == nullptr)
	{
		Logger::LogAndExit_SDL("SDL_GL_CreateContext Failed\n", SDL_GL_CREATE_CONTEXT_FAILED);
	}

	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)
	{
		Logger::LogAndExit("glewInit() Failed\n", GLEW_INIT_FAILED);
	}

	g_Keys = SDL_GetKeyboardState(NULL);
}

// Function to process SDL Events
bool SDLWindow::PollEvents()
{
	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
		case SDL_QUIT:
			return true;

		case SDL_KEYDOWN:
		#ifdef _DEBUG
			if (event.key.keysym.scancode == SDL_SCANCODE_F1)
			{
				if (!wireframe)
				{
					glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
					wireframe = true;
				}
				else
				{
					glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
					wireframe = false;
				}
			}

			if (event.key.keysym.scancode == SDL_SCANCODE_F2)
			{
				if (vsync)
				{
					SDL_GL_SetSwapInterval(0);
					vsync = false;
				}
				else
				{
					SDL_GL_SetSwapInterval(1);
					vsync = true;
				}
			}
		#endif

		default:
			break;
		}
	}

	return false;
}

// Free memory resources before exiting
SDLWindow::~SDLWindow()
{
	Logger::Log("\nQuiting SDL2", Logger::INFO);
	SDL_GL_DeleteContext(glContext);
	SDL_DestroyWindow(window);
	SDL_Quit();
}