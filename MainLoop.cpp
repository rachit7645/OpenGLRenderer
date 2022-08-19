#include "Window.h"

#include <vector>

#include "Model.h"
#include "Entity.h"
#include "Camera.h"
#include "Light.h"
#include "MasterRenderer.h"
#include "Terrain.h"
#include "Material.h"
#include "Player.h"
#include "MeshTextures.h"
#include "Resources.h"
#include "Imgui.h"
#include "GUI.h"
#include "WaterTile.h"
#include "FrameBuffer.h"

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
using Terrains::Terrain;
using Terrains::TerrainTextures;
using Waters::WaterTile;

// TODO: Move MainLoop to separate class, move data to said class
// TODO: Live editing of entities, terrains, lights, etc. with ImGui

constexpr auto MAX_ENTITIES = 100;

void SDLWindow::MainLoop()
{
	// Put Models and Textures here 
	auto treeTexture  = Resources::GetTexture("gfx/tree.png");
	auto grassTexture = Resources::GetTexture("gfx/grassTexture.png");
	auto fernTexture  = Resources::GetTexture("gfx/fern.png");
	auto defDiffuse   = Resources::GetTexture("gfx/dragon.png");
	auto defSpecular  = Resources::GetTexture("gfx/dSpec.png");

	TerrainTextures textures =
	{
		Resources::GetTexture("gfx/grass.png"),
		Resources::GetTexture("gfx/mud.png"),
		Resources::GetTexture("gfx/path.png"),
		Resources::GetTexture("gfx/pinkFlowers.png"),
		Resources::GetTexture("gfx/blendMap.png")
	};

	auto treeModel   = Resources::GetModel("gfx/tree.obj",       MeshTextures(treeTexture,  defSpecular));
	auto playerModel = Resources::GetModel("gfx/Link/Link.obj",  MeshTextures(defDiffuse,   defSpecular));
	auto grassModel  = Resources::GetModel("gfx/grassModel.obj", MeshTextures(grassTexture, defSpecular),  Material(true, true));
	auto fernModel   = Resources::GetModel("gfx/fern.obj",       MeshTextures(fernTexture,  defSpecular),  Material(true, true));

	// All objects go here
	std::vector<Terrain> terrains;
	{
		terrains.emplace_back("gfx/heightMap.png", glm::vec2(0.0f, 0.0f), textures);
	}

	std::vector<Entity> entities;
	{
		const Terrain* current;
		f32 entityX, entityY, entityZ;
		for (size_t i = 0; i < MAX_ENTITIES; ++i)
		{
			entityX = Util::Rand_Range<f32>(0.0f, 1.0f) * Terrains::TERRAIN_SIZE;
			entityZ = Util::Rand_Range<f32>(0.0f, 1.0f) * Terrains::TERRAIN_SIZE;
			current = Terrains::GetCurrent(terrains, glm::vec2(entityX, entityZ));
			entityY = current != nullptr ? current->GetHeight(glm::vec2(entityX, entityZ)) : 0.0f;
			entities.emplace_back(treeModel, glm::vec3(entityX, entityY, entityZ), glm::vec3(0.0f), 3.0f);

			entityX = Util::Rand_Range<f32>(0.0f, 1.0f) * Terrains::TERRAIN_SIZE;
			entityZ = Util::Rand_Range<f32>(0.0f, 1.0f) * Terrains::TERRAIN_SIZE;
			current = Terrains::GetCurrent(terrains, glm::vec2(entityX, entityZ));
			entityY = current != nullptr ? current->GetHeight(glm::vec2(entityX, entityZ)) : 0.0f;
			entities.emplace_back(fernModel, glm::vec3(entityX, entityY, entityZ), glm::vec3(0.0f), 0.6f);
		}
	}
	Player player
	(
		playerModel,
		glm::vec3(250.0f, 0.0f, 235.0f),
		glm::vec3(0.0f, 180.0f, 0.0f),
		1.0f
	);

	std::vector<Light> lights;
	{
		lights.emplace_back
		(
			glm::vec3(20000.0f, 20000.0f, 2000.0f),
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

	std::vector<GUI> guis;
	{
	}

	std::vector<WaterTile> waters;
	{
		waters.emplace_back(glm::vec3(185.0f, 3.2f, 240.0f));
	}

	Entities::Camera camera(&player);
	Renderer::MasterRenderer renderer;

	// Check FrameBuffers
	// auto fb = std::make_shared<FrameBuffer>(1024, 576, FBType::ColorAndDepth);

	startTime = frameStartTime = steady_clock::now();

	while (true)
	{
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplSDL2_NewFrame(window);
		ImGui::NewFrame();

		ImGuiDisplay();
		player.Move(Terrains::GetCurrent(terrains, player));
		camera.Move();

		renderer.RenderScene(entities, terrains, lights, camera, player);
		renderer.RenderWaters(waters);

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