#include "Window.h"

using namespace Window;
using namespace Entities;

// Main loop for the engine
void SDLWindow::MainLoop()
{
	InitGL();

	Renderer::Texture texture("res/textures/stallTexture.png");
	Renderer::Texture texture2("res/textures/tree.png");

	Renderer::Model model = Renderer::LoadModel("res/models/stall.obj", texture);
	model.shineDamper = 10.0f;
	model.reflectivity = 1.0f;
	Renderer::Model model2 = Renderer::LoadModel("res/models/tree.obj", texture2);
	
	std::vector<Entity> entities;
	entities.push_back(Entity(model, glm::vec3(0.0f, 0.0f, -20.0f), glm::vec3(0.0f, 0.0f, 0.0f), 1.0f));
	entities.push_back(Entity(model, glm::vec3(15.0f, 0.0f, -20.0f), glm::vec3(0.0f, 0.0f, 0.0f), 1.0f));
	entities.push_back(Entity(model2, glm::vec3(30.0f, 0.0f, -20.0f), glm::vec3(0.0f, 0.0f, 0.0f), 3.0f));
	entities.push_back(Entity(model2, glm::vec3(45.0f, 0.0f, -20.0f), glm::vec3(0.0f, 0.0f, 0.0f), 3.0f));

	Entities::Light light(glm::vec3(0.0f, 10.0f, -25.0f), glm::vec3(1.0f, 1.0f, 1.0f));
	Entities::Camera camera(glm::vec3(0.0f, 3.0f, 0.0f));
	Renderer::MasterRenderer renderer;

	startTime = SDL_GetTicks64();

	while (true)
	{
		camera.Move();
		
		for (auto& entity : entities)
		{
			entity.rotation.y += 1.0f * g_Delta;
			renderer.ProcessEntity(entity);
		}	
		renderer.Render(light, camera);

		SDL_GL_SwapWindow(window);
		CalculateFPS();
			
		if (PollEvents()) break;
	}
}

// Calculates the FPS and the the frame delta
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
	// GL Defs
	glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
	glEnable(GL_MULTISAMPLE);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
}