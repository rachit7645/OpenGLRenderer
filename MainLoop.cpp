#include "Window.h"
#include "MainLoop.h"

using namespace Window;

using std::chrono::steady_clock;
using std::chrono::duration_cast;
using std::chrono::milliseconds;
using std::chrono::seconds;

using Renderer::Texture;
using Renderer::Model;
using Renderer::Material;
using Renderer::MeshTextures;
using Entities::Entity;
using Entities::Player;
using Entities::Skybox;
using Entities::Light;
using Terrains::Terrain;
using Terrains::TerrainTextures;

// TODO: Move MainLoop to separate class, move data to said class
// TODO: Live editing of entities, terrains, lights, etc. with ImGui

void SDLWindow::MainLoop()
{
	// Put Models and Textures here 
	auto treeTexture = std::make_shared<Texture>("gfx/tree.png");
	auto grassTexture = std::make_shared<Texture>("gfx/grassTexture.png");
	auto fernTexture = std::make_shared<Texture>("gfx/fern.png");
	auto defDiffuse = std::make_shared<Texture>("gfx/dragon.png");
	auto defSpecular = std::make_shared<Texture>("gfx/dSpec.png");

	TerrainTextures textures =
	{
		std::make_shared<Texture>("gfx/grass.png"),
		std::make_shared<Texture>("gfx/mud.png"),
		std::make_shared<Texture>("gfx/path.png"),
		std::make_shared<Texture>("gfx/pinkFlowers.png"),
		std::make_shared<Texture>("gfx/blendMap.png")
	};


	auto treeModel = std::make_shared<Model>("gfx/tree.obj", MeshTextures(treeTexture, defSpecular));
	auto playerModel = std::make_shared<Model>("gfx/Link/Link.obj", MeshTextures(defDiffuse, defSpecular));
	auto grassModel = std::make_shared<Model>("gfx/grassModel.obj", MeshTextures(grassTexture, defSpecular), Material(true, true));
	auto fernModel = std::make_shared<Model>("gfx/fern.obj", MeshTextures(fernTexture, defSpecular), Material(true, true));

	// All objects go here
	std::vector<Terrain> terrains;
	{
		terrains.push_back(Terrain("gfx/heightMap.png", glm::vec2(0.0f, 0.0f), textures));
	}

	std::vector<Entity> entities;
	{
		const Terrain* current = nullptr;
		f32 entityX, entityY, entityZ;
		for (size_t i = 0; i < 100; ++i)
		{
			entityX = Util::Rand_Range<f32>(0.0f, 1.0f) * Terrains::TERRAIN_SIZE;
			entityZ = Util::Rand_Range<f32>(0.0f, 1.0f) * Terrains::TERRAIN_SIZE;
			current = Terrains::GetCurrent(terrains, glm::vec2(entityX, entityZ));
			entityY = current != nullptr ? current->GetHeight(glm::vec2(entityX, entityZ)) : 0.0f;
			entities.push_back(Entity(treeModel, glm::vec3(entityX, entityY, entityZ), glm::vec3(0.0f, 0.0f, 0.0f), 3.0f));

			entityX = Util::Rand_Range<f32>(0.0f, 1.0f) * Terrains::TERRAIN_SIZE;
			entityZ = Util::Rand_Range<f32>(0.0f, 1.0f) * Terrains::TERRAIN_SIZE;
			current = Terrains::GetCurrent(terrains, glm::vec2(entityX, entityZ));
			entityY = current != nullptr ? current->GetHeight(glm::vec2(entityX, entityZ)) : 0.0f;
			entities.push_back(Entity(fernModel, glm::vec3(entityX, entityY, entityZ), glm::vec3(0.0f, 0.0f, 0.0f), 0.6f));
		}
	}
	Player player(playerModel, glm::vec3(250.0f, 0.0f, 235.0f), glm::vec3(0.0f, 180.0f, 0.0f), 1.0f);

	std::vector<Light> lights;
	{
		lights.push_back(Light
		(
			glm::vec3(20000.0f, 20000.0f, 2000.0f),
			glm::vec3(0.2f, 0.2f, 0.2f),
			glm::vec3(0.3f, 0.3f, 0.3f),
			glm::vec3(1.0f, 1.0f, 1.0f),
			glm::vec3(1.0f, 0.0f, 0.0f)
		));
		lights.push_back(Light
		(
			glm::vec3(250.0f, 5.0f, 235.0f),
			glm::vec3(0.2f, 0.2f, 0.2f),
			glm::vec3(0.0f, 2.0f, 2.0f),
			glm::vec3(1.0f, 1.0f, 1.0f),
			glm::vec3(1.0f, 0.01f, 0.002f)
		));
	};

	Entities::Skybox skybox;
	Entities::Camera camera(&player);

	Renderer::MasterRenderer renderer;
	// The skybox doesn't really change, so it is loaded once in the start
	renderer.ProcessSkybox(skybox);

	startTime = frameStartTime = steady_clock::now();

	while (true)
	{
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplSDL2_NewFrame(window);
		ImGui::NewFrame();

		ImGuiDisplay();
		player.Move(Terrains::GetCurrent(terrains, player));
		camera.Move();

		for (const auto& entity : entities)
		{
			renderer.ProcessEntity(entity);
		}
		renderer.ProcessEntity(player);

		for (const auto& terrain : terrains)
		{
			renderer.ProcessTerrain(terrain);
		}
		renderer.Render(lights, camera);

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		SDL_GL_SwapWindow(window);

		CalculateFPS();
		if (PollEvents()) break;
	}
}

void SDLWindow::CalculateFPS()
{
	endTime = steady_clock::now();
	g_Delta = duration_cast<milliseconds>(endTime - frameStartTime).count() / 1000.0;
	frameStartTime = endTime;

	if (endTime - startTime >= seconds(1))
	{
		startTime = endTime;
		finalFPS = FPS;
		frameTime = 1000.0f / FPS;
		FPS = 0.0f;
	}
	FPS++;
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

void SDLWindow::ImGuiUpdate()
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