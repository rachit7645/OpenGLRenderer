#include "Window.h"
#include "MainLoop.h"

using namespace Window;

using Renderer::Texture;
using Renderer::Model;
using Renderer::Material;
using Entities::Entity;
using Entities::Player;
using Terrains::Terrain;
using Terrains::TerrainTextures;

void SDLWindow::MainLoop()
{
	InitGL();

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

	startTime = frameStartTime = SDL_GetTicks64();

	while (true)
	{
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

		SDL_GL_SwapWindow(window);
		CalculateFPS();

		if (PollEvents()) break;
	}
}

void SDLWindow::CalculateFPS()
{
	endTime = SDL_GetTicks64();
	g_Delta = (endTime - frameStartTime) / 1000.0f;
	frameStartTime = endTime;

	if (endTime >= startTime + 1000)
	{
		std::cerr << "\rFPS: " << FPS;
		startTime = endTime;
		FPS = 0.0f;
	}
	FPS++;
}

void SDLWindow::InitGL()
{
	glViewport(0, 0, DIMENSIONS.x, DIMENSIONS.y);
	glEnable(GL_MULTISAMPLE);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
}