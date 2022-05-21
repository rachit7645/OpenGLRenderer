#include "Window.h"

using namespace Window;

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
		SDL_WINDOWPOS_UNDEFINED, DIMENSIONS.x, DIMENSIONS.y, SDL_WINDOW_FLAGS
	);
	if (window == nullptr)
	{
		Logger::LogAndExit_SDL("SDL_CreateWindow Failed\n", SDL_CREATE_WINDOW_FAILED);
	}
	// For sanity, raise window
	SDL_RaiseWindow(window);
	SDL_ShowCursor(SDL_FALSE);
	SDL_SetRelativeMouseMode(SDL_TRUE);
	g_Keys = SDL_GetKeyboardState(nullptr);

	// Basically useless GLContext
	// You don't get more than GL 1.1 for compatibilty reasons (Windows YOU SUCK)
	glContext = SDL_GL_CreateContext(window);
	if (glContext == nullptr)
	{
		Logger::LogAndExit_SDL("SDL_GL_CreateContext Failed\n", SDL_GL_CREATE_CONTEXT_FAILED);
	}

	// Initialize the REAL OpenGL context
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)
	{
		Logger::LogAndExit("glewInit Failed\n", GLEW_INIT_FAILED);
	}

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	UNUSED ImGuiIO& io = ImGui::GetIO();
	ImGui::StyleColorsDark();

	ImGui_ImplSDL2_InitForOpenGL(window, glContext);
	ImGui_ImplOpenGL3_Init("#version 330 core");

	InitGL();
}

bool SDLWindow::PollEvents()
{
	while (SDL_PollEvent(&event))
	{
		ImGui_ImplSDL2_ProcessEvent(&event);

		switch (event.type)
		{
		case SDL_QUIT:
			return true;

		case SDL_KEYDOWN:
			switch (event.key.keysym.scancode)
			{
			case SDL_SCANCODE_F1:
				if (isInputCaptured)
				{
					SDL_SetRelativeMouseMode(SDL_FALSE);
					isInputCaptured = !isInputCaptured;
				}
				else
				{
					SDL_SetRelativeMouseMode(SDL_TRUE);
					isInputCaptured = !isInputCaptured;
				}
				break;
			default:
				break;
			}
			break;

		case SDL_MOUSEWHEEL:
			g_MouseScrollY = event.wheel.y;
			g_ToZoomCamera = true;
			break;

		case SDL_MOUSEMOTION:
			g_MousePos.x = event.motion.xrel;
			g_MousePos.y = event.motion.yrel;
			g_ToMoveCamera = true;
			break;

		default:
			break;
		}
	}

	return false;
}

void SDLWindow::InitGL()
{
	glViewport(0, 0, DIMENSIONS.x, DIMENSIONS.y);
	glEnable(GL_MULTISAMPLE);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
}

SDLWindow::~SDLWindow()
{
	Logger::Log("Quiting SDL2", Logger::INFO);
	
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();

	SDL_GL_DeleteContext(glContext);
	SDL_DestroyWindow(window);
	SDL_Quit();
}