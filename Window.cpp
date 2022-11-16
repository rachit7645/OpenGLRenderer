#include "Window.h"

#include <string_view>
#include <GL/glew.h>

#include "imgui.h"
#include "Log.h"
#include "Camera.h"
#include "Files.h"
#include "Inputs.h"
#include "Resources.h"
#include "GL.h"

using namespace Window;

constexpr u32 SDL_WINDOW_FLAGS  = SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN;

SDLWindow::SDLWindow()
{
	// Get SDL version
	SDL_version version = {};
	SDL_GetVersion(&version);
	LOG_INFO
	(
		"Initializing SDL2 version: {}.{}.{}\n",
		static_cast<usize>(version.major),
		static_cast<usize>(version.minor),
		static_cast<usize>(version.patch)
	);

	if (SDL_Init(SDL_INIT_VIDEO) != 0)
	{
		LOG_ERROR("SDL_Init Failed\n{}\n", SDL_GetError());
	}

	LOG_INFO("{}\n", "Setting up OpenGL context");
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	// OpenGL Version 4.3 Core
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);

	// RGBA8888 + Depth24 Framebuffer
	SDL_GL_SetAttribute(SDL_GL_RED_SIZE,   8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE,  8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

	window = SDL_CreateWindow
	(
		"Rachit's Engine",
		SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED,
		WINDOW_DIMENSIONS.x,
		WINDOW_DIMENSIONS.y,
		SDL_WINDOW_FLAGS
	);

	if (window == nullptr)
	{
		LOG_ERROR("SDL_CreateWindow Failed\n{}\n", SDL_GetError());
	}
	LOG_INFO("Created SDL_Window with address: {}\n", reinterpret_cast<void*>(window));
	
	// For sanity, raise window
	SDL_RaiseWindow(window);
	SDL_ShowCursor(SDL_FALSE);
	SDL_SetRelativeMouseMode(SDL_TRUE);

	// Basically useless GLContext
	// You don't get more than GL 1.1 for compatibility reasons (Windows YOU SUCK)
	glContext = SDL_GL_CreateContext(window);
	if (glContext == nullptr)
	{
		LOG_ERROR("SDL_GL_CreateContext Failed\n{}\n", SDL_GetError());
	}

	if (SDL_GL_MakeCurrent(window, glContext) != 0)
	{
		LOG_ERROR("SDL_GL_MakeCurrent Failed\n{}\n", SDL_GetError());
	}
	LOG_INFO("Created SDL_GLContext with address: {}\n", reinterpret_cast<void*>(&glContext));

	// Initialize the REAL OpenGL context
	auto glewVersion = reinterpret_cast<const char*>(glewGetString(GLEW_VERSION));
	LOG_INFO("Initializing GLEW version: {}\n", std::string_view(glewVersion));
	// Due to a bug in glew, set it to experimental mode
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)
	{
		LOG_ERROR("{}\n", "glewInit Failed");
	}
	GL::LogDebugInfo();

	LOG_INFO("Initializing Dear ImGui version: {}\n", ImGui::GetVersion());
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	UNUSED ImGuiIO& io = ImGui::GetIO();
	ImGui::StyleColorsDark();

	ImGui_ImplSDL2_InitForOpenGL(window, glContext);
	ImGui_ImplOpenGL3_Init("#version 430 core");

	Files::SetResourceDirectory("../res/");

	Inputs::Init();
	GL::Init(WINDOW_DIMENSIONS);
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
			Inputs::GetMouseScroll() = glm::ivec2(event.wheel.x, event.wheel.y);
			g_ToZoomCamera = true;
			break;

		case SDL_MOUSEMOTION:
			Inputs::GetMousePos() = glm::ivec2(event.motion.xrel, event.motion.yrel);
			g_ToMoveCamera = true;
			break;

		default:
			break;
		}
	}
	return false;
}

SDLWindow::~SDLWindow()
{
	LOG_INFO("{}\n", "Quiting SDL2");

	Resources::Delete();

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();

	SDL_GL_DeleteContext(glContext);
	SDL_DestroyWindow(window);
	SDL_Quit();
}