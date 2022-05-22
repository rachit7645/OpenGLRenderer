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
using Entities::Entity;
using Entities::Player;
using Terrains::Terrain;
using Terrains::TerrainTextures;

void SDLWindow::MainLoop()
{
	// Put Models and Textures here 
	std::shared_ptr<Texture> treeTexture = std::make_shared<Texture>("res/gfx/tree.png");
	std::shared_ptr<Texture> grassTexture = std::make_shared<Texture>("res/gfx/grassTexture.png");
	std::shared_ptr<Texture> fernTexture = std::make_shared<Texture>("res/gfx/fern.png");
	std::shared_ptr<Texture> defaultTexture = std::make_shared<Texture>("res/gfx/dragon.png");

	TerrainTextures textures = TerrainTextures(
		std::make_shared<Texture>("res/gfx/grass.png"),
		std::make_shared<Texture>("res/gfx/mud.png"),
		std::make_shared<Texture>("res/gfx/path.png"),
		std::make_shared<Texture>("res/gfx/pinkFlowers.png"),
		std::make_shared<Texture>("res/gfx/blendMap.png")
	);


	std::shared_ptr<Model> treeModel = std::make_shared<Model>("res/gfx/tree.obj", treeTexture);
	std::shared_ptr<Model> playerModel = std::make_shared<Model>("res/gfx/Link/Link.obj", defaultTexture);
	std::shared_ptr<Model> grassModel = std::make_shared<Model>("res/gfx/grassModel.obj", grassTexture, Material(true, true));
	std::shared_ptr<Model> fernModel = std::make_shared<Model>("res/gfx/fern.obj", fernTexture, Material(true, true));

	// All objects go here
	std::vector<Entity> entities;
	{
		for (size_t i = 0; i < 40; i++)
		{
			entities.push_back(Entity(treeModel, glm::vec3(Util::Rand_Range<f32>(0.0f, 1.0f) * 400 - 200,
				0, Util::Rand_Range<f32>(0.0f, 1.0f) * -300), glm::vec3(0.0f, 0.0f, 0.0f), 3.0f));
			entities.push_back(Entity(grassModel, glm::vec3(Util::Rand_Range<f32>(0.0f, 1.0f) * 400 - 200,
				0, Util::Rand_Range<f32>(0.0f, 1.0f) * -300), glm::vec3(0.0f, 0.0f, 0.0f), 1.0f));
			entities.push_back(Entity(fernModel, glm::vec3(Util::Rand_Range<f32>(0.0f, 1.0f) * 400 - 200,
				0, Util::Rand_Range<f32>(0.0f, 1.0f) * -300), glm::vec3(0.0f, 0.0f, 0.0f), 0.6f));
		}
	}
	Player player(playerModel, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 180.0f, 0.0f), 1.0f);

	std::vector<Terrain> terrains;
	{
		terrains.push_back(Terrain(glm::vec2(0.0f, -1.0f), textures));
		terrains.push_back(Terrain(glm::vec2(-1.0f, -1.0f), textures));
		terrains.push_back(Terrain(glm::vec2(0.0f, 0.0f), textures));
		terrains.push_back(Terrain(glm::vec2(-1.0f, 0.0f), textures));
	}

	Entities::Light light(glm::vec3(20000.0f, 20000.0f, 2000.0f), glm::vec3(1.0f, 1.0f, 1.0f));
	Entities::Camera camera(player);
	Renderer::MasterRenderer renderer;

	startTime = frameStartTime = steady_clock::now();

	while (true)
	{
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplSDL2_NewFrame(window);
		ImGui::NewFrame();

		ImGui::Begin("Engine");
		ImGui::Text("FPS: %.2f", finalFPS);
		ImGui::Text("Frame time: %.2f ms", frameTime);
		ImGui::Checkbox("Vsync", &vsync);
		ImGui::Checkbox("Wireframe", &wireframe);
		ImGui::End();

		ImGuiUpdate();
		player.Move();
		camera.Move();

		for (const auto &entity : entities)
		{
			renderer.ProcessEntity(entity);
		}
		renderer.ProcessEntity(player);

		for (const auto &terrain : terrains)
		{
			renderer.ProcessTerrain(terrain);
		}
		renderer.Render(light, camera);

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