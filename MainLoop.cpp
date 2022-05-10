#include "Window.h"
#include "MainLoop.h"

using namespace Window;

using Entities::Entity;
using Terrains::Terrain;
using Renderer::Texture;
using Renderer::Model;
using Renderer::Material;

void SDLWindow::MainLoop()
{
	InitGL();

	// Put Models and Textures here 
	std::shared_ptr<Texture> treeTexture = std::make_shared<Texture>("res/textures/tree.png");
	std::shared_ptr<Texture> grassTexture = std::make_shared<Texture>("res/textures/grassTexture.png");
	std::shared_ptr<Texture> fernTexture = std::make_shared<Texture>("res/textures/fern.png");

	std::shared_ptr<Texture> backgroundTexture = std::make_shared<Texture>("res/textures/grass.png");
	std::shared_ptr<Texture> rTexture = std::make_shared<Texture>("res/textures/mud.png");
	std::shared_ptr<Texture> gTexture = std::make_shared<Texture>("res/textures/path.png");
	std::shared_ptr<Texture> bTexture = std::make_shared<Texture>("res/textures/pinkFlowers.png");
	std::shared_ptr<Texture> blendMap = std::make_shared<Texture>("res/textures/blendMap.png");

	std::shared_ptr<Model> treeModel = std::make_shared<Model>("res/models/tree.obj", Material(), treeTexture);
	std::shared_ptr<Model> grassModel = std::make_shared<Model>("res/models/grassModel.obj", Material(true, true), grassTexture);
	std::shared_ptr<Model> fernModel = std::make_shared<Model>("res/models/fern.obj", Material(true, true), fernTexture);

	// All objects go here
	std::vector<Entity> entities;
	{
		for (size_t i = 0; i < 40; i++)
		{
			entities.push_back(Entity(treeModel, glm::vec3(Rand_Range<f32>(0.0f, 1.0f) * 400 - 200,
				0, Rand_Range<f32>(0.0f, 1.0f) * -300), glm::vec3(0.0f, 0.0f, 0.0f), 3.0f));
			entities.push_back(Entity(grassModel, glm::vec3(Rand_Range<f32>(0.0f, 1.0f) * 400 - 200,
				0, Rand_Range<f32>(0.0f, 1.0f) * -300), glm::vec3(0.0f, 0.0f, 0.0f), 1.0f));
			entities.push_back(Entity(fernModel, glm::vec3(Rand_Range<f32>(0.0f, 1.0f) * 400 - 200,
				0, Rand_Range<f32>(0.0f, 1.0f) * -300), glm::vec3(0.0f, 0.0f, 0.0f), 0.6f));
		}
	}
	std::vector<Terrain> terrains;
	{
		std::array<std::shared_ptr<Texture>, Terrains::TEXTURE_COUNT> textures 
			= { backgroundTexture, rTexture, gTexture, bTexture, blendMap };
		terrains.push_back(Terrain(glm::vec2(0.0f, -1.0f), Material(), textures));
		terrains.push_back(Terrain(glm::vec2(-1.0f, -1.0f), Material(), textures));
		terrains.push_back(Terrain(glm::vec2(0.0f, 0.0f), Material(), textures));
		terrains.push_back(Terrain(glm::vec2(-1.0f, 0.0f), Material(), textures));
	}

	Entities::Light light(glm::vec3(20000.0f, 20000.0f, 2000.0f), glm::vec3(1.0f, 1.0f, 1.0f));
	Entities::Camera camera(glm::vec3(0.0f, 6.0f, 0.0f));
	Renderer::MasterRenderer renderer;

	startTime = SDL_GetTicks64();

	while (true)
	{
		camera.Move();
		for (const auto &entity : entities)
		{
			renderer.ProcessEntity(entity);
		}
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
	if ((endTime = SDL_GetTicks64()) >= startTime + 1000)
	{
		std::cerr << "\rFPS: " << FPS;
		FPS = 0;
		g_Delta = (endTime - startTime) / 1000.0f;
		startTime = endTime;
		return;
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