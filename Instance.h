#ifndef ENGINE_INSTANCE_H
#define ENGINE_INSTANCE_H

#include <vector>
#include <chrono>

#include "Entity.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"
#include "GUI.h"
#include "WaterTile.h"
#include "Util.h"
#include "Player.h"
#include "Camera.h"
#include "RenderManager.h"
#include "Window.h"

namespace Engine
{
	class Instance
	{
	public:
		// Vector tags
		using EntityVec  = std::vector<Entities::Entity>;
		using GUIs       = std::vector<Renderer::GUI>;
		using WaterTiles = std::vector<Waters::WaterTile>;
		// Lights vector tags
		using DirLights   = std::vector<Entities::DirectionalLight>;
		using PointLights = std::vector<Entities::PointLight>;
		using SpotLights  = std::vector<Entities::SpotLight>;
		// Time tags
		using Clock     = std::chrono::steady_clock;
		using TimePoint = std::chrono::time_point<Clock>;

		// Main constructor
		Instance();

		// Run engine
		void Run();
	private:
		// Display ImGui Widgets
		void ImGuiDisplay();
		// Update menu
		void ImGuiUpdate();
		// Calculates the FPS and the frame delta
		void CalculateFPS();

		// Initialise entities (and player)
		void InitEntities();
		// Initialise all lights
		void InitLights();
		// Initialise misc.
		void InitMisc();

		// SDL handle
		Engine::Window m_window;

		// Entity vector
		EntityVec m_entities = {};
		// GUIs vector
		GUIs m_guis = {};
		// Water tiles
		WaterTiles m_waters = {};

		// Directional lights vector
		DirLights m_dirLights = {};
		// Point lights vector
		PointLights m_pointLights = {};
		// Spot lights vector
		SpotLights m_spotLights = {};

		// Player
		Entities::Player m_player;
		// Camera
		Entities::Camera m_camera;

		// Renderer
		Renderer::RenderManager m_renderer = {};

		// Cycle start time
		TimePoint m_startTime = {};
		// Frame start time
		TimePoint m_frameStartTime = {};
		// Frame end time
		TimePoint m_endTime = {};

		// Current frame time
		f32 m_frameTime = 0.0f;
		// Current FPS
		f32 m_FPS = 0.0f;
		// ImGui FPS
		f32 m_finalFPS = 0.0f;

		// Wireframe mode flag
		bool m_wireframe = false;
		// (Adaptive) V-SYNC flag
		bool m_vsync = true;
		// Selected directional light
		s32 m_selectedDirLight = 0;
		// Selected point light
		s32 m_selectedPointLight = 0;
		// Selected spot light
		s32 m_selectedSpotLight = 0;

		// Current polygon mode
		GLenum m_currentPolyMode = GL_FILL;
		// Current swap mode
		s32 m_currentSwapMode = -1;
	};
}

#endif