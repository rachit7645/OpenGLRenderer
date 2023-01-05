#include "Window.h"

#include <GL/glew.h>

#include "ImGui.h"
#include "Log.h"
#include "Camera.h"
#include "Files.h"
#include "Inputs.h"
#include "Resources.h"
#include "GL.h"
#include "Settings.h"

// Using namespaces
using namespace Engine;

// Usings
using Entities::Camera;

// Window flags
constexpr u32 SDL_WINDOW_FLAGS  = SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN;

Window::Window()
{
	// Get settings
	const auto& settings = Settings::GetInstance();

	// Get SDL version
	SDL_version version = {};
	SDL_GetVersion(&version);

	// Log it
	LOG_INFO
	(
		"Initializing SDL2 version: {}.{}.{}\n",
		static_cast<usize>(version.major),
		static_cast<usize>(version.minor),
		static_cast<usize>(version.patch)
	);

	// Initialise SDL2
	if (SDL_Init(SDL_INIT_VIDEO) != 0)
	{
		// Log error
		LOG_ERROR("SDL_Init Failed\n{}\n", SDL_GetError());
	}

	// Set up opengl context
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	// OpenGL Version 4.3 Core
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);

	// RGBA8 + D24 Framebuffer
	SDL_GL_SetAttribute(SDL_GL_RED_SIZE,     8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE,    8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE,   8);
	SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE,   8);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE,   24);

	// Create SDL handle
	handle = SDL_CreateWindow
	(
		"Rachit's Engine",
		SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED,
		settings.window.dimensions.x,
		settings.window.dimensions.y,
		SDL_WINDOW_FLAGS
	);

	// If handle creation failed
	if (handle == nullptr)
	{
		// Log error
		LOG_ERROR("SDL_CreateWindow Failed\n{}\n", SDL_GetError());
	}
	
	// For sanity, raise handle
	SDL_RaiseWindow(handle);
	// Set mouse mode
	SDL_ShowCursor(SDL_FALSE);
	SDL_SetRelativeMouseMode(SDL_TRUE);

	// Create dummy GL context
	m_glContext = SDL_GL_CreateContext(handle);

	// If context creation fails
	if (m_glContext == nullptr)
	{
		// Log error
		LOG_ERROR("SDL_GL_CreateContext Failed\n{}\n", SDL_GetError());
	}

	// Make the context current
	if (SDL_GL_MakeCurrent(handle, m_glContext) != 0)
	{
		// Log error
		LOG_ERROR("SDL_GL_MakeCurrent Failed\n{}\n", SDL_GetError());
	}

	// Initialize the real OpenGL context
	LOG_INFO("Initializing GLEW version: {}\n", GLEW::GetString(GLEW_VERSION));
	// Due to a bug in glew, set it to experimental mode
	glewExperimental = GL_TRUE;
	// If GLEW init fails
	if (glewInit() != GLEW_OK)
	{
		// Log error
		LOG_ERROR("{}\n", "glewInit Failed");
	}

	// Initialise Dear ImGui
	LOG_INFO("Initializing Dear ImGui version: {}\n", ImGui::GetVersion());
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	UNUSED ImGuiIO& io = ImGui::GetIO();
	ImGui::StyleColorsDark();

	// Initialise ImGui backend
	ImGui_ImplSDL2_InitForOpenGL(handle, m_glContext);
	ImGui_ImplOpenGL3_Init("#version 430 core");

	// Set resource directory
	auto& files = Files::GetInstance();
	files.SetResources("../res/");
	// Initialise other GL things
	GL::Init(settings.window.dimensions);
}

bool Window::PollEvents()
{
	// While events exist
	while (SDL_PollEvent(&m_event))
	{
		// ImGUI process event
		ImGui_ImplSDL2_ProcessEvent(&m_event);

		// Switch event types
		switch (m_event.type)
		{
		// Event to quit
		case SDL_QUIT:
			return true;

		// Key event
		case SDL_KEYDOWN:
			switch (m_event.key.keysym.scancode)
			{
			// If F1 is pressed
			case SDL_SCANCODE_F1:
				// Toggle mouse mode
				SDL_SetRelativeMouseMode(static_cast<SDL_bool>(!m_isInputCaptured));
				// Toggle flag
				m_isInputCaptured = !m_isInputCaptured;
				break;
			// Default key handler
			default:
				break;
			}
			break;

		// Mouse scroll event
		case SDL_MOUSEWHEEL:
			// Store scroll position
			Inputs::GetInstance().GetMouseScroll() = glm::ivec2(m_event.wheel.x, m_event.wheel.y);
			// Set camera zoom flag
			Camera::GetToZoomCamera() = true;
			break;

		// Mouse motion event
		case SDL_MOUSEMOTION:
			// Store mouse position
			Inputs::GetInstance().GetMousePos() = glm::ivec2(m_event.motion.xrel, m_event.motion.yrel);
			// Set camera movement flag
			Camera::GetToMoveCamera() = true;
			break;

		// Default event handler
		default:
			break;
		}
	}

	// Return
	return false;
}

Window::~Window()
{
	// Clear resources
	Resources::GetInstance().Clear();

	// Close ImGUI
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();

	// CLose SDL
	SDL_GL_DeleteContext(m_glContext);
	SDL_DestroyWindow(handle);
	SDL_Quit();
}