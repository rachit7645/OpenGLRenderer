#ifndef SDL_WINDOW_H
#define SDL_WINDOW_H

#include <vector>
#include <chrono>
#include <SDL2/SDL.h>

#include "GLM.h"
#include "Util.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"

namespace Engine
{
	class Window
	{
	public:
		// Usings
		using Clock     = std::chrono::steady_clock;
		using TimePoint = std::chrono::time_point<Clock>;
		// Creates a SDL window, gets an opengl context, and sets up GL values
		Window();
		// Free SDL memory resources
		~Window();
		// Main loop for the engine
		void MainLoop();
	private:
		// Display ImGui Widgets
		void ImGuiDisplay
		(
			std::vector<Entities::DirectionalLight>& dirLights,
			std::vector<Entities::PointLight>& pointLights,
			std::vector<Entities::SpotLight>& spotLights
		);
		// Update menu
		void ImGuiUpdate();
		// Function to process SDL Events
		bool PollEvents();
		// Calculates the m_FPS and the frame delta
		void CalculateFPS();

		// Time variables
		TimePoint m_startTime      = {};
		TimePoint m_frameStartTime = {};
		TimePoint m_endTime        = {};

		// m_FPS counters
		f32 m_frameTime = 0.0f;
		f32 m_FPS       = 0.0f;
		f32 m_finalFPS  = 0.0f;

		// ImGui state
		bool m_wireframe          = false;
		bool m_vsync              = true;
		bool m_isInputCaptured    = true;
		s32  m_selectedDirLight   = 0;
		s32  m_selectedPointLight = 0;
		s32  m_selectedSpotLight  = 0;

		// Other state
		GLenum m_currentPolyMode = GL_FILL;
		s32    m_currentSwapMode = -1;

		// SDL resourcesPath
		SDL_Window*   m_window    = nullptr;
		SDL_GLContext m_glContext = nullptr;
		SDL_Event     m_event     = {};
	};

}

#endif