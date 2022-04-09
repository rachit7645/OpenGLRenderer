#include "Window.h"

using namespace Window;

// Main loop for the engine
void SDLWindow::MainLoop()
{
	InitGL();

	Renderer::Texture texture("res/textures/stallTexture.png");
	Renderer::Model model = Renderer::LoadModel("res/models/stall.obj", texture);
	model.shineDamper = 10.0f;
	model.reflectivity = 1.0f;
	
	Entities::Entity entity(model, glm::vec3(0.0f, 0.0f, -20.0f), glm::vec3(0.0f, 0.0f, 0.0f), 1.0f);
	Entities::Light light(glm::vec3(0.0f, 10.0f, -25.0f), glm::vec3(1.0f, 1.0f, 1.0f));
	Entities::Camera camera(glm::vec3(0.0f, 3.0f, 0.0f));

	Shader::StaticShader shader("res/shaders/vertexShader.glsl", "res/shaders/fragmentShader.glsl"); 
	Renderer::GLRenderer renderer;

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
		shader.LoadLight(light);
		renderer.Render(entity, shader);
		shader.Stop();

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

inline void SDLWindow::CreateProjectionMatrix()
{
	projectionMatrix = glm::perspective(glm::radians(FOV), ASPECT_RATIO, NEAR_PLANE, FAR_PLANE);
}