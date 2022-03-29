#include "Window.h"

using namespace Window;

// Main loop for the engine
void SDLWindow::MainLoop()
{
	InitGL();

	Renderer::Model model = Renderer::LoadModel("res/models/stall.obj", "res/textures/stallTexture.png");
	Entities::Entity entity(model, glm::vec3(0.0f, 0.0f, -12.0f), glm::vec3(0.0f, 0.0f, 0.0f), 1.0f);

	Shader::StaticShader shader("res/shaders/vertexShader.glsl", "res/shaders/fragmentShader.glsl"); 
	Renderer::GLRenderer renderer;

	Entities::Camera camera(glm::vec3(0.0f, 3.0f, 0.0f));

	CreateProjectionMatrix();
	shader.Start();
	shader.LoadProjectionMatrix(projectionMatrix);
	shader.Stop();

	startTime = SDL_GetTicks64();

	while (true)
	{
		entity.rotation.y += 1.0f * g_Delta;
		camera.Move();
		renderer.Prepare();
		shader.Start();
		shader.LoadViewMatrix(camera);
		renderer.Render(entity, shader);
		shader.Stop();

		SDL_GL_SwapWindow(window);
		CalculateFPS();
			
		if (PollEvents()) break;
	}
	CleanUp();
}

// Calculates the FPS and the the frame delta
void SDLWindow::CalculateFPS()
{
	if ( (endTime = SDL_GetTicks64()) >= startTime + 1000 )
	{
		std::cout << "\rFPS: " << FPS;
		FPS = 0;
		g_Delta = (endTime - startTime) / 1000;
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

inline void SDLWindow::CreateProjectionMatrix()
{
	projectionMatrix = glm::perspective(glm::radians(FOV), ASPECT_RATIO, NEAR_PLANE, FAR_PLANE);
}