#include "Window.h"

#include <vector>

#include "Model.h"
#include "Entity.h"
#include "Camera.h"
#include "Light.h"
#include "MasterRenderer.h"
#include "Material.h"
#include "Player.h"
#include "MeshTextures.h"
#include "Resources.h"
#include "imgui.h"
#include "GUI.h"
#include "WaterTile.h"
#include "WaterFrameBuffers.h"

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
using Renderer::FBType;
using Entities::Entity;
using Entities::Player;
using Entities::Skybox;
using Entities::Light;
using Waters::WaterTile;

// TODO: Move MainLoop to separate class, move data to said class
// TODO: Live editing of entities, lights, etc. with ImGui

constexpr usize MAX_ENTITIES    = 25;
constexpr f32   ENTITY_DISTANCE = 196.0f;

void SDLWindow::MainLoop()
{
	// Put Models and Textures here 
	auto treeTexture  = Resources::GetTexture("gfx/tree.png");
	auto grassTexture = Resources::GetTexture("gfx/grassTexture.png");
	auto fernTexture  = Resources::GetTexture("gfx/fern.png");
	auto defDiffuse   = Resources::GetTexture("gfx/dragon.png");
	auto defSpecular  = Resources::GetTexture("gfx/dSpec.png");

	auto treeModel   = Resources::GetModel("gfx/tree.obj",       MeshTextures(treeTexture,  defSpecular));
	auto playerModel = Resources::GetModel("gfx/Link/Link.obj",  MeshTextures(defDiffuse,   defSpecular));
	auto grassModel  = Resources::GetModel("gfx/grassModel.obj", MeshTextures(grassTexture, defSpecular),  Material(true, true));
	auto fernModel   = Resources::GetModel("gfx/fern.obj",       MeshTextures(fernTexture,  defSpecular),  Material(true, true));

	// All objects go here
	std::vector<Entity> entities;
	{
		f32 entityX, entityZ;
		for (usize i = 0; i < MAX_ENTITIES; ++i)
		{
			entityX = Util::Rand_Range<f32>(-1.0f, 1.0f) * ENTITY_DISTANCE;
			entityZ = Util::Rand_Range<f32>(-1.0f, 1.0f) * ENTITY_DISTANCE;
			entities.emplace_back(treeModel, glm::vec3(entityX, 0.0f, entityZ), glm::vec3(0.0f), 3.0f);

			entityX = Util::Rand_Range<f32>(0.0f, 1.0f) * ENTITY_DISTANCE;
			entityZ = Util::Rand_Range<f32>(0.0f, 1.0f) * ENTITY_DISTANCE;
			entities.emplace_back(fernModel, glm::vec3(entityX, 0.0f, entityZ), glm::vec3(0.0f), 0.6f);
		}
	}
	Player player
	(
		playerModel,
		glm::vec3(67.0f, 0.0f, 73.0f),
		glm::vec3(0.0f, 180.0f, 0.0f),
		1.0f
	);

	std::vector<Light> lights;
	{
		lights.emplace_back
		(
			glm::vec3(0.0f, 1000.0f, -7000.0f),
			glm::vec3(0.2f, 0.2f, 0.2f),
			glm::vec3(0.3f, 0.3f, 0.3f),
			glm::vec3(1.0f, 1.0f, 1.0f),
			glm::vec3(1.0f, 0.0f, 0.0f)
		);
		lights.emplace_back
		(
			glm::vec3(250.0f, 5.0f, 235.0f),
			glm::vec3(0.2f, 0.2f, 0.2f),
			glm::vec3(0.0f, 2.0f, 2.0f),
			glm::vec3(1.0f, 1.0f, 1.0f),
			glm::vec3(1.0f, 0.01f, 0.002f)
		);
	}

	// Check FrameBuffers
	auto waterFBOs = Waters::WaterFrameBuffers();

	std::vector<GUI> guis;
	{
		/* Use for debugging
		guis.emplace_back
		(
			waterFBOs.reflectionFBO->colorTexture,
			glm::vec2(-0.5f, 0.5f),
			glm::vec2(0.5f, 0.5f)
		);

		guis.emplace_back
		(
			waterFBOs.refractionFBO->colorTexture,
			glm::vec2(-0.5f, -0.5f),
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

	Entities::Camera camera(&player);
	Renderer::MasterRenderer renderer;

	startTime = frameStartTime = steady_clock::now();

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
		renderer.BeginFrame(entities, lights, player);

		// Enable clip plane 0
		glEnable(GL_CLIP_DISTANCE0);

		// Reflection pass
		waterFBOs.BindReflection();
		// Move the camera
		f32 distance = 2.0f * (camera.position.y - waters[0].position.y);
		camera.position.y -= distance;
		camera.InvertPitch();
		renderer.RenderScene(camera, glm::vec4(0.0f, 1.0f, 0.0f, -waters[0].position.y));
		// Move it back to its original position
		camera.position.y += distance;
		camera.InvertPitch();

		// Refraction pass
		waterFBOs.BindRefraction();
		renderer.RenderScene(camera, glm::vec4(0.0f, -1.0f, 0.0f, waters[0].position.y));

		// Disable clip plane 0
		glDisable(GL_CLIP_DISTANCE0);

		// Main render pass
		waterFBOs.BindDefaultFBO();
		renderer.RenderScene(camera);
		renderer.RenderWaters(waters, waterFBOs);
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
		ImGui::EndMainMenuBar();
	}
	ImGuiUpdate();
}

void SDLWindow::ImGuiUpdate() const
{
	if (wireframe)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}
	else
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}

	if (!vsync)
	{
		SDL_GL_SetSwapInterval(0);
	}
	else
	{
		SDL_GL_SetSwapInterval(1);
	}
}