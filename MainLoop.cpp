#include "Window.h"

#include "Model.h"
#include "Entity.h"
#include "Player.h"
#include "MeshTextures.h"
#include "Resources.h"
#include "imgui.h"
#include "GUI.h"
#include "WaterTile.h"
#include "RenderManager.h"
#include "Camera.h"

using namespace Window;

using std::chrono::steady_clock;
using std::chrono::duration_cast;
using std::chrono::milliseconds;
using std::chrono::seconds;

using Renderer::Texture;
using Renderer::Model;
using Renderer::Material;
using Renderer::MeshTextures;
using Renderer::GUI;
using Renderer::FrameBuffer;
using Renderer::RenderManager;
using Renderer::Mode;
using Entities::Entity;
using Entities::Skybox;
using Entities::Light;
using Entities::Camera;
using Waters::WaterTile;

// TODO: Move MainLoop to separate class, move data to said class
// TODO: Live editing of entities, etc. with ImGui

void SDLWindow::MainLoop()
{
	// Put Models and Textures here
	auto defDiffuse  = Resources::GetTexture("gfx/def.png");
	auto defSpecular = Resources::GetTexture("gfx/def.png");

	auto playerModel = Resources::GetModel("gfx/Link/Link.obj",   MeshTextures(defDiffuse, defSpecular));
	auto stageModel  = Resources::GetModel("gfx/Stage/stage.obj", MeshTextures(defDiffuse, defSpecular));
	auto stallModel  = Resources::GetModel("gfx/Stall/stall.obj", MeshTextures(defDiffuse, defSpecular));

	// All objects go here
	std::vector<Entity> entities;
	{
		entities.emplace_back
		(
			stageModel,
			glm::vec3(0.0f, -0.6f, 0.0f),
			glm::vec3(0.0f),
			1.0f
		);

		entities.emplace_back
		(
			stallModel,
			glm::vec3(-3.0f, 0.0f, -15.0f),
			glm::vec3(0.0f, 90.0f, 0.0f),
			1.0f
		);
	}

	auto player = Entities::Player
	(
		playerModel,
		glm::vec3(13.0f, 0.0f, 17.0f),
		glm::vec3(0.0f, 180.0f, 0.0f),
		1.0f
	);

	std::vector<Light> lights;
	{
		lights.emplace_back
		(
			glm::vec3(-2.0f, 4.0f, -1.0f),
			glm::vec3(0.2f, 0.2f, 0.2f),
			glm::vec3(0.4f, 0.4f, 0.4f),
			glm::vec3(1.0f, 1.0f, 1.0f),
			glm::vec3(1.0f, 0.0f, 0.0f)
		);
	}

	std::vector<GUI> guis;
	{
		/* guis.emplace_back
		(
			shadowFBO.buffer->depthTexture,
			glm::vec2(-0.5f, 0.5f),
			glm::vec2(0.5f, 0.5f)
		); */
	}

	std::vector<WaterTile> waters;
	{
		waters.emplace_back
		(
			Resources::GetTexture("gfx/waterDUDV.png"),
			Resources::GetTexture("gfx/normal.png"),
			glm::vec3(120.0f, 3.7f, -2.0f)
		);
	}

	auto camera   = Entities::Camera(&player);
	auto renderer = Renderer::RenderManager();

	startTime = frameStartTime = steady_clock::now();

	while (true)
	{
		// Prepare ImGUI
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplSDL2_NewFrame(window);
		ImGui::NewFrame();

		// Update
		ImGuiDisplay(lights);
		player.Move();
		camera.Move();

		// Begin render
		renderer.BeginFrame(entities, lights, player);
		// Draw shadow framebuffer
		renderer.RenderShadows(camera, lights[0]);
		// Draw water framebuffers
		renderer.RenderWaterFBOs(waters, camera);

		// Deferred geometry pass
		renderer.RenderGBuffer(camera);
		// Deferred lighting pass
		renderer.RenderLighting(camera);

		// Copy depth from gBuffer
		renderer.CopyDepth();
		// Render waters
		renderer.RenderWaters(waters);
		// Render skybox
		renderer.RenderSkybox();
		// Render GUIs
		renderer.RenderGUIs(guis);
		// End render
		renderer.EndFrame();

		// ImGUI render pass
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		SDL_GL_SwapWindow(window);
		CalculateFPS();
		if (PollEvents()) break;
	}
}

void SDLWindow::CalculateFPS()
{
	endTime       = steady_clock::now();
	auto duration = duration_cast<milliseconds>(endTime - frameStartTime);
	g_Delta       = static_cast<f32>(static_cast<f64>(duration.count()) / 1000.0);

	frameStartTime = endTime;
	if (endTime - startTime >= seconds(1))
	{
		startTime = endTime;
		finalFPS  = FPS;
		frameTime = static_cast<f32>(1000.0 / FPS);
		FPS       = 0.0f;
	}
	++FPS;
}

void SDLWindow::ImGuiDisplay(std::vector<Light>& lights)
{
	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("Options"))
		{
			ImGui::Text("FPS: %.2f", finalFPS);
			ImGui::Text("Frame time: %.2f ms", frameTime);
			ImGui::Checkbox("Vsync", &vsync);
			ImGui::Checkbox("Wireframe", &wireframe);
			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Editor"))
		{
			if (ImGui::BeginMenu("Lights"))
			{
				static int current = 0;
				static const char* const items[] =
				{
					"[0]",
					"[1]",
					"[2]",
					"[3]"
				};

				ImGui::Combo("Current", &current, items, IM_ARRAYSIZE(items));
				ImGui::InputFloat3("Position",    &lights[current].position[0],    "%.1f");
				ImGui::InputFloat3("Ambient",     &lights[current].ambient[0],     "%.1f");
				ImGui::InputFloat3("Diffuse",     &lights[current].diffuse[0],     "%.1f");
				ImGui::InputFloat3("Specular",    &lights[current].specular[0],    "%.1f");
				ImGui::InputFloat3("Attenuation", &lights[current].attenuation[0], "%.1f");
				ImGui::EndMenu();
			}

			ImGui::EndMenu();
		}

		ImGui::EndMainMenuBar();
	}
	ImGuiUpdate();
}

void SDLWindow::ImGuiUpdate() const
{
	glPolygonMode(GL_FRONT_AND_BACK, wireframe ? GL_LINE : GL_FILL);
	SDL_GL_SetSwapInterval(vsync);
}