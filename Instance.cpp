#include "Instance.h"

#include "Resources.h"
#include "MeshTextures.h"

// Using namespaces
using namespace Engine;

// Namespace aliases
namespace chrono = std::chrono;

// Usings
using Renderer::MeshTextures;

Instance::Instance()
{
	InitEntities();
	InitLights();
	InitMisc();
}

void Instance::Run()
{
	// Set start time
	m_startTime = m_frameStartTime = chrono::steady_clock::now();

	while (true)
	{
		// Prepare ImGui
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplSDL2_NewFrame(m_window.handle);
		ImGui::NewFrame();

		// Update
		ImGuiDisplay();
		m_player.Move();
		m_camera.Move();

		// Begin render
		m_renderer.BeginFrame(m_entities, m_dirLights, m_pointLights, m_spotLights, m_player);
		// Draw shadow framebuffer
		m_renderer.RenderShadows(m_camera, m_dirLights[0].position);
		// Draw water framebuffers
		m_renderer.RenderWaterFBOs(m_waters, m_camera);

		// Deferred geometry pass
		m_renderer.RenderGBuffer(m_camera);
		// Deferred lighting pass
		m_renderer.RenderLighting(m_camera);

		// Render waters
		m_renderer.RenderWaters(m_waters);
		// Render skybox
		m_renderer.RenderSkybox();
		// End render
		m_renderer.EndFrame();

		// ImGui render pass
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		// Swap handle
		SDL_GL_SwapWindow(m_window.handle);
		// Calculate FPS
		CalculateFPS();

		// Poll events
		if (m_window.PollEvents()) break;
	}
}

void Instance::CalculateFPS()
{
	// Calculate end time
	m_endTime = chrono::steady_clock::now();
	// Calculate frame duration
	auto duration = chrono::duration_cast<chrono::milliseconds>(m_endTime - m_frameStartTime);
	// Calculate frame delta
	g_Delta = static_cast<f32>(static_cast<f64>(duration.count()) / 1000.0);
	// Set this/next frame's start time
	m_frameStartTime = m_endTime;

	// If a second has passed
	if (m_endTime - m_startTime >= chrono::seconds(1))
	{
		// Set this cycle's start time
		m_startTime = m_endTime;
		// Update displayed FPS
		m_finalFPS  = m_FPS;
		// Calculate frame time
		m_frameTime = static_cast<f32>(1000.0 / m_FPS);
		// Reset FPS
		m_FPS = 0.0f;
	}

	// Increment FPS
	++m_FPS;
}

void Instance::ImGuiDisplay()
{
	// If menu bar is visible
	if (ImGui::BeginMainMenuBar())
	{
		// If options menu is selected
		if (ImGui::BeginMenu("Options"))
		{
			// Display m_FPS info
			ImGui::Text("FPS: %.2f", m_finalFPS);
			ImGui::Text("Frame time: %.2f ms", m_frameTime);
			// Display options
			ImGui::Checkbox("Vsync", &m_vsync);
			ImGui::Checkbox("Wireframe", &m_wireframe);
			// End menu
			ImGui::EndMenu();
		}

		// If editor menu is selected
		if (ImGui::BeginMenu("Editor"))
		{
			// If lights menu is selected
			if (ImGui::BeginMenu("Lights"))
			{
				// Shared indices
				constexpr std::array<const char*, 4> indices = {"[0]", "[1]", "[2]", "[3]"};

				// If directional lights menu is selected
				if (ImGui::BeginMenu("Directional"))
				{
					// Light selector
					ImGui::Combo("Slot", &m_selectedDirLight, indices.data(), indices.size());
					// Select light
					auto& light = m_dirLights[m_selectedDirLight];
					// Position
					ImGui::InputFloat3("Position", &light.position[0], "%.1f");
					// Color
					ImGui::ColorEdit3("Color", &light.color[0]);
					// Intensity
					ImGui::DragFloat3("Intensity", &light.intensity[0], 0.5f, 0.0f, 100.0f, "%.1f");
					// End menu
					ImGui::EndMenu();
				}

				// If point lights menu is selected
				if (ImGui::BeginMenu("Point"))
				{
					// Light selector
					ImGui::Combo("Slot", &m_selectedPointLight, indices.data(), indices.size());
					// Select light
					auto& light = m_pointLights[m_selectedPointLight];
					// Position
					ImGui::InputFloat3("Position", &light.position[0], "%.1f");
					// Color
					ImGui::ColorEdit3("Color", &light.color[0]);
					// Intensity
					ImGui::DragFloat3("Intensity", &light.intensity[0], 0.5f, 0.0f, 100.0f, "%.1f");
					// Attenuation
					ImGui::InputFloat3("Attenuation", &light.attenuation[0], "%.4f");
					// End menu
					ImGui::EndMenu();
				}

				// If spot lights menu is selected
				if (ImGui::BeginMenu("Spot"))
				{
					// Light selector
					ImGui::Combo("Slot", &m_selectedSpotLight, indices.data(), indices.size());
					// Select light
					auto& light = m_spotLights[m_selectedSpotLight];
					// Position
					ImGui::InputFloat3("Position", &light.position[0], "%.1f");
					// Color
					ImGui::ColorEdit3("Color", &light.color[0]);
					// Intensity
					ImGui::DragFloat3("Intensity", &light.intensity[0], 0.5f, 0.0f, 100.0f, "%.1f");
					// Attenuation
					ImGui::InputFloat3("Attenuation", &light.attenuation[0], "%.4f");
					// Direction
					ImGui::InputFloat3("Direction", &light.direction[0], "%.1f");
					// Get cut off in degrees
					auto degCutOff = light.GetCutOff();
					// Cut off
					ImGui::DragFloat2("CutOff", &degCutOff[0], 0.5f, 0.0f, 180.0f, "%.1f");
					// Set new cut off
					light.SetCutOff(degCutOff);
					// End menu
					ImGui::EndMenu();
				}

				// End menu
				ImGui::EndMenu();
			}

			// End menu
			ImGui::EndMenu();
		}

		// End menu bar
		ImGui::EndMainMenuBar();
	}

	ImGuiUpdate();
}

void Instance::ImGuiUpdate()
{
	// Calculate selections
	GLenum selectedPolyMode = m_wireframe ? GL_LINE : GL_FILL;
	s32    selectedSwapMode = m_vsync ? -1 : 0;

	// Wireframe check
	if (selectedPolyMode != m_currentPolyMode)
	{
		glPolygonMode(GL_FRONT_AND_BACK, selectedPolyMode);
		m_currentPolyMode = selectedPolyMode;
	}

	// V-SYNC check
	if (selectedSwapMode != m_currentSwapMode)
	{
		SDL_GL_SetSwapInterval(selectedSwapMode);
		m_currentSwapMode = selectedSwapMode;
	}
}

void Instance::InitEntities()
{
	// Get resource handle
	auto& resources = Resources::GetInstance();

	// Default textures
	auto defaultTextures = MeshTextures
	(
		resources.GetTexture("gfx/def.png"),
		resources.GetTexture("gfx/defNrm.png"),
		resources.GetTexture("gfx/def.png")
	);

	// All models
	auto playerModel  = resources.GetModel("gfx/Mario/Mario.gltf",     defaultTextures);
	auto cottageModel = resources.GetModel("gfx/Cottage/Cottage.gltf", defaultTextures);
	auto benchModel   = resources.GetModel("gfx/Bench/Bench.gltf",     defaultTextures);
	auto boxModel     = resources.GetModel("gfx/Box/scene.gltf",       defaultTextures);

	// Entities
	m_entities =
	{
		{
			cottageModel,
			glm::vec3(0.0f),
			glm::vec3(0.0f),
			50.0f
		},
		{
			benchModel,
			glm::vec3(44.0f, 0.0f, -40.0f),
			glm::vec3(0.0f, -90.0f, 0.0f),
			5.0f
		},
		{
			boxModel,
			glm::vec3(-44.0f, 3.6f, -45.0f),
			glm::vec3(-90.0f, 0.0f, 0.0f),
			2.0f
		}
	};

	// Player
	m_player =
	{
		playerModel,
		glm::vec3(13.0f, 2.3f, 17.0f),
		glm::vec3(0.0f, 180.0f, 0.0f),
		0.03f
	};

	// Attach player
	m_camera.player = &m_player;
}

void Instance::InitLights()
{
	// Directional lights
	m_dirLights =
	{
		{
			glm::vec3(0.1f, 1.0f, 0.1f),
			glm::vec3(0.3f, 0.3f, 0.3f),
			glm::vec3(1.0f, 1.0f, 1.0f)
		}
	};

	// Point lights
	m_pointLights =
	{
		{
			glm::vec3(40.0f, 4.0f, -40.0f),
			glm::vec3(0.0f, 1.0f, 1.0f),
			glm::vec3(1.0f, 7.0f, 5.0f),
			glm::vec3(1.0f, 0.022f, 0.0019f)
		}
	};

	// Spot lights
	m_spotLights =
	{
		{
			glm::vec3(-40.0f, 4.0f, 40.0f),
			glm::vec3(1.0f, 0.0f, 0.0f),
			glm::vec3(10.0f, 1.0f, 1.0f),
			glm::vec3(1.0f, 0.007f, 0.0002f),
			glm::vec3(0.0f, 0.0f, -0.8f),
			glm::vec2(10.0f, 25.0f)
		}
	};
}

void Instance::InitMisc()
{
	// Get resource handle
	auto& resources = Resources::GetInstance();

	// Waters
	m_waters =
	{
		{
			resources.GetTexture("gfx/waterDUDV.png"),
			resources.GetTexture("gfx/normal.png"),
			glm::vec3(120.0f, 3.7f, -2.0f)
		}
	};
}