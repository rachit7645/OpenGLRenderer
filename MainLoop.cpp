#include "Window.h"

using namespace Window;

// Main loop for the engine
void SDLWindow::MainLoop()
{
	InitGL();

	// Test Vertices
	std::vector<f32> vertices = 
	{
		 -0.5f,0.5f,-0.5f,	
		-0.5f,-0.5f,-0.5f,	
		0.5f,-0.5f,-0.5f,	
		0.5f,0.5f,-0.5f,		
		
		-0.5f,0.5f,0.5f,	
		-0.5f,-0.5f,0.5f,	
		0.5f,-0.5f,0.5f,	
		0.5f,0.5f,0.5f,
				
		0.5f,0.5f,-0.5f,	
		0.5f,-0.5f,-0.5f,	
		0.5f,-0.5f,0.5f,	
		0.5f,0.5f,0.5f,
				
		-0.5f,0.5f,-0.5f,	
		-0.5f,-0.5f,-0.5f,	
		-0.5f,-0.5f,0.5f,	
		-0.5f,0.5f,0.5f,

		-0.5f,0.5f,0.5f,
		-0.5f,0.5f,-0.5f,
		0.5f,0.5f,-0.5f,
		0.5f,0.5f,0.5f,
		
		-0.5f,-0.5f,0.5f,
		-0.5f,-0.5f,-0.5f,
		0.5f,-0.5f,-0.5f,
		0.5f,-0.5f,0.5f
	};

	std::vector<u32> indices =
	{
		0,1,3,	
		3,1,2,	
		4,5,7,
		7,5,6,
		8,9,11,
		11,9,10,
		12,13,15,
		15,13,14,	
		16,17,19,
		19,17,18,
		20,21,23,
		23,21,22
	};

	std::vector<f32> textureCoords =
	{
    	0,0,
		0,1,
		1,1,
		1,0,			
		0,0,
		0,1,
		1,1,
		1,0,			
		0,0,
		0,1,
		1,1,
		1,0,
		0,0,
		0,1,
		1,1,
		1,0,
		0,0,
		0,1,
		1,1,
		1,0,
		0,0,
		0,1,
		1,1,
		1,0
	};

	Renderer::Model model(vertices, indices, textureCoords, "res/textures/stone.png");
	Entities::Entity entity(model, glm::vec3(0.0f, 0.0f, -1.5f), glm::vec3(0.0f, 0.0f, 0.0f), 1.0f);

	Shader::StaticShader shader("res/shaders/vertexShader.glsl", "res/shaders/fragmentShader.glsl"); 
	Renderer::GLRenderer renderer;

	Entities::Camera camera(glm::vec3(0.0f, 0.0f, 0.0f));

	CreateProjectionMatrix();
	shader.Start();
	shader.LoadProjectionMatrix(projectionMatrix);
	shader.Stop();

	startTime = SDL_GetTicks64();

	while (true)
	{
		entity.rotation.y += 0.1f;
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
		delta = (endTime - startTime) / 1000;
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
}

inline void SDLWindow::CreateProjectionMatrix()
{
	projectionMatrix = glm::perspective(glm::radians(FOV), ASPECT_RATIO, NEAR_PLANE, FAR_PLANE);
}