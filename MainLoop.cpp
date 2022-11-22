#include "Window.h"

#include <vector>

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
#include "PointLight.h"

using namespace Window;

// Namespace alias
namespace chrono = std::chrono;

using Renderer::Texture;
using Renderer::Model;
using Renderer::MeshTextures;
using Renderer::GUI;
using Renderer::FrameBuffer;
using Renderer::RenderManager;
using Renderer::Mode;
using Entities::Entity;
using Entities::Skybox;
using Entities::PointLight;
using Entities::Camera;
using Waters::WaterTile;

// TODO: Move MainLoop to separate class, move data to said class
// TODO: Live editing of entities, etc. with ImGui

void SDLWindow::MainLoop()
{
	// Put Models and Textures here
	auto defaultTextures = MeshTextures
	(
		Resources::GetTexture("gfx/def.png"),
		Resources::GetTexture("gfx/defNrm.png"),
		Resources::GetTexture("gfx/def.png")
	);

	auto playerModel  = Resources::GetModel("gfx/Mario/Mario.gltf",     defaultTextures);
	auto cottageModel = Resources::GetModel("gfx/Cottage/Cottage.gltf", defaultTextures);
	auto benchModel   = Resources::GetModel("gfx/Bench/Bench.gltf",     defaultTextures);

	// All objects go here
	std::vector<Entity> entities;
	{
		entities.emplace_back
		(
			cottageModel,
			glm::vec3(0.0f),
			glm::vec3(0.0f),
			50.0f
		);

		entities.emplace_back
		(
			benchModel,
			glm::vec3(44.0f, 0.0f, 40.0f),
			glm::vec3(0.0f, -90.0f, 0.0f),
			5.0f
		);
	}

	auto player = Entities::Player
	(
		playerModel,
		glm::vec3(13.0f, 6.0f, 17.0f),
		glm::vec3(0.0f, 180.0f, 0.0f),
		1.0f
	);

	std::vector<PointLight> pointLights;
	{
		pointLights.emplace_back
		(
			glm::vec3(0.1f, 1.0f, 0.1f),
			glm::vec3(1.0f, 1.0f, 1.0f),
			glm::vec3(1.0f, 0.0f, 0.0f)
		);
	}

	std::vector<GUI> guis;
	{
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

	startTime = frameStartTime = chrono::steady_clock::now();

	while (true)
	{
		// Prepare ImGUI
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplSDL2_NewFrame(window);
		ImGui::NewFrame();

		// Update
		ImGuiDisplay();
		player.Move();
		camera.Move();

		// Begin render
		renderer.BeginFrame(entities, pointLights, player);
		// Draw shadow framebuffer
		renderer.RenderShadows(camera, pointLights[0].position);
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
	endTime       = chrono::steady_clock::now();
	auto duration = chrono::duration_cast<chrono::milliseconds>(endTime - frameStartTime);
	g_Delta       = static_cast<f32>(static_cast<f64>(duration.count()) / 1000.0);

	frameStartTime = endTime;
	if (endTime - startTime >= chrono::seconds(1))
	{
		startTime = endTime;
		finalFPS  = FPS;
		frameTime = static_cast<f32>(1000.0 / FPS);
		FPS       = 0.0f;
	}
	++FPS;
}

void SDLWindow::ImGuiDisplay()
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

		// TODO: Add back lights editor

		ImGui::EndMainMenuBar();
	}

	ImGuiUpdate();
}

void SDLWindow::ImGuiUpdate()
{
	GLenum selectedPolyMode = wireframe ? GL_LINE : GL_FILL;
	s32    selectedSwapMode = vsync     ? -1      : 0;

	if (selectedPolyMode != currentPolyMode)
	{
		glPolygonMode(GL_FRONT_AND_BACK, selectedPolyMode);
		currentPolyMode = selectedPolyMode;
	}

	if (selectedSwapMode != currentSwapMode)
	{
		SDL_GL_SetSwapInterval(selectedSwapMode);
		currentSwapMode = selectedSwapMode;
	}
}