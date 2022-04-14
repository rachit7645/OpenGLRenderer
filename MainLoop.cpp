#include "Window.h"

using namespace Window;
using namespace Entities;

void SDLWindow::MainLoop()
{
	InitGL();

	// Put Models and Textures here 
	std::shared_ptr<Renderer::Texture> texture = std::make_shared<Renderer::Texture>("res/textures/stallTexture.png");
	std::shared_ptr<Renderer::Texture> texture2 = std::make_shared<Renderer::Texture>("res/textures/tree.png");
	std::shared_ptr<Renderer::Model> model = std::make_shared<Renderer::Model>(Renderer::LoadModel("res/models/stall.obj", texture));
	{
		model->shineDamper = 10.0f;
		model->reflectivity = 1.0f;
	}
	std::shared_ptr<Renderer::Model> model2 = std::make_shared<Renderer::Model>(Renderer::LoadModel("res/models/tree.obj", texture2));

	// All objects go here
	std::vector<Entity> entities;
	{
		entities.push_back(Entity(model, glm::vec3(-15.0f, 0.0f, -20.0f), glm::vec3(0.0f, 0.0f, 0.0f), 1.0f));
		entities.push_back(Entity(model, glm::vec3(0.0f, 0.0f, -20.0f), glm::vec3(0.0f, 0.0f, 0.0f), 1.0f));
		entities.push_back(Entity(model, glm::vec3(15.0f, 0.0f, -20.0f), glm::vec3(0.0f, 0.0f, 0.0f), 1.0f));
		entities.push_back(Entity(model2, glm::vec3(30.0f, 0.0f, -20.0f), glm::vec3(0.0f, 0.0f, 0.0f), 3.0f));
		entities.push_back(Entity(model2, glm::vec3(45.0f, 0.0f, -20.0f), glm::vec3(0.0f, 0.0f, 0.0f), 3.0f));
		entities.push_back(Entity(model2, glm::vec3(60.0f, 0.0f, -20.0f), glm::vec3(0.0f, 0.0f, 0.0f), 3.0f));
	}
	Entities::Light light(glm::vec3(-30.0f, 10.0f, -25.0f), glm::vec3(1.0f, 1.0f, 1.0f));
	Entities::Camera camera(glm::vec3(0.0f, 3.0f, 0.0f));

	Renderer::MasterRenderer renderer;
	startTime = SDL_GetTicks64();

	while (true)
	{
		camera.Move();
		for (auto& entity : entities)
		{
			entity.rotation.y += 2 * g_Delta;
			renderer.ProcessEntity(entity);
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
		std::cout << "\rFPS: " << FPS;
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