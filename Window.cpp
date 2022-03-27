#include "Window.h"

using namespace Window;

// Main loop for the engine
void SDLWindow::MainLoop()
{
	// Test Vertices
	std::vector<f32> vertices = 
	{
		 0.5f,  0.5f, 0.0f,  // top right
    	 0.5f, -0.5f, 0.0f,  // bottom right
    	-0.5f, -0.5f, 0.0f,  // bottom left
    	-0.5f,  0.5f, 0.0f   // top left
	};
	std::vector<u32> indices =
	{
		0, 1, 3,   // first triangle
    	1, 2, 3    // second triangle
	};
	std::vector<f32> textureCoords =
	{
    	0, 0,
		0, 1,
		1, 1,
		1, 0
	};

	Renderer::Model model(vertices, indices, textureCoords, "res/textures/stone.png");
	Renderer::GLRenderer renderer;
	Shader::StaticShader shader("res/shaders/vertexShader.glsl", "res/shaders/fragmentShader.glsl"); 
	startTime = SDL_GetTicks64();

	while (true)
	{
		renderer.Prepare();
		shader.Start();
		renderer.Render(model);
		shader.Stop();

		SDL_GL_SwapWindow(window);
		CalculateFPS();
			
		if (PollEvents()) break;
	}
	CleanUp();
}

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
		SDL_WINDOWPOS_UNDEFINED, WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_FLAGS
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

	SDL_GL_SetSwapInterval(0);

	// GL Defs
	glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
	glEnable(GL_MULTISAMPLE);

}

// Calculates the FPS and the the frame delta
void SDLWindow::CalculateFPS()
{
	if ( (endTime = SDL_GetTicks64()) >= startTime + 1000 )
	{
		std::cout << "\rFPS: " << FPS;
		FPS = 0;
		delta = (endTime - startTime) / 1000;
		startTime = endTime;
		return;
	}
	FPS++;
}

// Function to process SDL Events
bool SDLWindow::PollEvents()
{
	if (SDL_PollEvent(&event))
	{
		if (event.type == SDL_QUIT)
			return true;
	}
	return false;
}

// Free memory resources before exiting
void SDLWindow::CleanUp()
{
	Logger::Log("\nQuiting SDL2", Logger::INFO);
	SDL_GL_DeleteContext(glContext);
	SDL_DestroyWindow(window);
	SDL_Quit();
}