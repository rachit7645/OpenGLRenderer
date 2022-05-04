#include "Window.h"
#include "MainLoop.h"

using namespace Window;

using Entities::Entity;
using Terrains::Terrain;
using Renderer::Texture;
using Renderer::Model;

void SDLWindow::MainLoop()
{
	InitGL();

	// Put Models and Textures here 
	std::shared_ptr<Texture> texture = std::make_shared<Texture>("res/textures/tree.png");
	std::shared_ptr<Texture> terrainTexture = std::make_shared<Texture>("res/textures/grass.png");
	std::shared_ptr<Model> model = std::make_shared<Model>("res/models/tree.obj", texture);

	// All objects go here
	std::vector<Entity> entities;
	{
		// Epic seed, I know
		std::srand(144);
		for (size_t i = 0; i < 100; i++)
		{
			entities.push_back(Entity(model, glm::vec3(Rand_Range<f32>(0.0f, 1.0f) * 400 - 200,
				0, Rand_Range<f32>(0.0f, 1.0f) * -300), glm::vec3(0.0f, 0.0f, 0.0f), 3.0f));
		}
	}
	std::vector<Terrain> terrains;
	{
		terrains.push_back(Terrain(glm::vec2(0.0f, -1.0f), terrainTexture));
		terrains.push_back(Terrain(glm::vec2(-1.0f, -1.0f), terrainTexture));
		terrains.push_back(Terrain(glm::vec2(0.0f, 0.0f), terrainTexture));
		terrains.push_back(Terrain(glm::vec2(-1.0f, 0.0f), terrainTexture));
	}
	
	Entities::Light light(glm::vec3(20000.0f, 20000.0f, 2000.0f), glm::vec3(1.0f, 1.0f, 1.0f));
	Entities::Camera camera(glm::vec3(0.0f, 6.0f, 0.0f));
	Renderer::MasterRenderer renderer;

	startTime = SDL_GetTicks64();

	while (true)
	{
		camera.Move();
		for (const auto& entity : entities)
		{
			renderer.ProcessEntity(entity);
		}
		for (const auto& terrain : terrains)
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
	if ( (endTime = SDL_GetTicks64()) >= startTime + 1000 )
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