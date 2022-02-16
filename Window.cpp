#include "Window.h"
#include "Model.h"

// Global definitions

SDL_Window* window = nullptr;
SDL_GLContext glContext = nullptr;
SDL_Event event;

u64 startTime, endTime;
f64 FPS;

// Main loop for the engine
void Window::MainLoop()
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

	glm::vec3 color(0.0f);

	Renderer::Model model = Renderer::CreateModel(vertices, indices);
	Shader::StaticShader shader("res/shaders/vertexShader.glsl", "res/shaders/fragmentShader.glsl"); 
	startTime = SDL_GetTicks64();

	while (true)
	{
		f64 timeValue = SDL_GetTicks64() / (f64) 1000;
		color.r = sin(timeValue) / 2.0f + 0.5f;
		color.g = cos(timeValue) / 2.0f + 0.5f;
		color.b = tan(timeValue) / 2.0f + 0.5f;

		Renderer::Prepare();
		shader.program.Start();
		shader.LoadColor(color);
		Renderer::Render(model);
		shader.program.Stop();

		SDL_GL_SwapWindow(window);
		CalculateFPS();
			
		if (PollEvents()) break;
	}
	shader.program.CleanUP();
}

// Creates a SDL window, gets an opengl context and jumps to the main loop
void Window::CreateWindow()
{
	Logger::Log("Initializing SDL2\n", Logger::INFO);
	if (SDL_Init(SDL_INIT_VIDEO) != 0)
	{
		Logger::LogAndExit_SDL("SDL_Init Failed\n", 1);
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
		Logger::LogAndExit_SDL("SDL_CreateWindow Failed\n", 2);
	}
	// For sanity, raise window
	SDL_RaiseWindow(window);

	// Basically useless GLContext
	// You don't get more than GL 1.1 for compatibilty reasons (Windows YOU SUCK)
	glContext = SDL_GL_CreateContext(window);
	if (glContext == nullptr)
	{
		Logger::LogAndExit_SDL("SDL_GL_CreateContext Failed\n", 3);
	}

	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)
	{
		Logger::LogAndExit("glewInit() Failed\n", 4);
	}

	// GL Defs
	glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
	glEnable(GL_MULTISAMPLE);

	MainLoop();

	Logger::Log("\nQuiting SDL2", Logger::INFO);
	CleanUp();

}

// Calculates the FPS and the the frame delta
void Window::CalculateFPS()
{

	if ( (endTime = SDL_GetTicks64()) >= startTime + 1000 )
	{
		std::cout << "\rFPS: " << FPS;
		FPS = 0;
		Window::delta = (endTime - startTime) / 1000;
		startTime = endTime;
	}
	else
	{
		FPS++;
	}

}

// Function to process SDL Events
bool Window::PollEvents()
{
	if (SDL_PollEvent(&event))
	{
		if (event.type == SDL_QUIT)
			return true;
	}
	return false;
}

// Free memory resources before exiting
void Window::CleanUp()
{
	SDL_GL_DeleteContext(glContext);
	SDL_DestroyWindow(window);
	SDL_Quit();
}