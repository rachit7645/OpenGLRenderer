#include "Window.h"

#include "Model.h"
#include "Entity.h"
#include "Light.h"
#include "Material.h"
#include "Player.h"
#include "MeshTextures.h"
#include "Resources.h"
#include "imgui.h"
#include "GUI.h"
#include "Random.h"
#include "ShadowFrameBuffer.h"

using namespace Window;

using std::chrono::steady_clock;
using std::chrono::duration_cast;
using std::chrono::milliseconds;
using std::chrono::seconds;

using Renderer::Texture;
using Renderer::Model;
using Renderer::Material;
using Renderer::MeshTextures;
using Renderer::GUI;
using Renderer::FrameBuffer;
using Renderer::FBType;
using Renderer::MasterRenderer;
using Renderer::Mode;
using Entities::Entity;
using Entities::Player;
using Entities::Skybox;
using Entities::Light;
using Entities::Camera;
using Waters::WaterTile;
using Waters::WaterFrameBuffers;

// TODO: Move MainLoop to separate class, move data to said class
// TODO: Live editing of entities, lights, etc. with ImGui

constexpr usize MAX_ENTITIES    = 50;
constexpr f32   ENTITY_DISTANCE = 256.0f;

void SDLWindow::MainLoop()
{
	// Put Models and Textures here 
	auto treeTexture  = Resources::GetTexture("gfx/tree.png");
	auto defDiffuse   = Resources::GetTexture("gfx/dragon.png");
	auto defSpecular  = Resources::GetTexture("gfx/dSpec.png");

	auto treeModel   = Resources::GetModel("gfx/tree.obj",       MeshTextures(treeTexture,  defSpecular));
	auto playerModel = Resources::GetModel("gfx/Link/Link.obj",  MeshTextures(defDiffuse,   defSpecular));

	// All objects go here
	std::vector<Entity> entities;
	{
		for (usize i = 0; i < MAX_ENTITIES; ++i)
		{
			auto entityX = Util::RandRange(-1.0f, 1.0f) * ENTITY_DISTANCE;
			auto entityZ = Util::RandRange(-1.0f, 1.0f) * ENTITY_DISTANCE;

			entities.emplace_back
			(
				treeModel,
				glm::vec3(entityX, 0.0f, entityZ),
				glm::vec3(0.0f),
				3.0f
			);
		}
	}

	Player player
	(
		playerModel,
		glm::vec3(67.0f, 0.0f, 73.0f),
		glm::vec3(0.0f, 180.0f, 0.0f),
		1.0f
	);

	std::vector<Light> lights;
	{
		lights.emplace_back
		(
			glm::vec3(0.0f, 1000.0f, -7000.0f),
			glm::vec3(0.2f, 0.2f, 0.2f),
			glm::vec3(0.3f, 0.3f, 0.3f),
			glm::vec3(1.0f, 1.0f, 1.0f),
			glm::vec3(1.0f, 0.0f, 0.0f)
		);
		lights.emplace_back
		(
			glm::vec3(250.0f, 5.0f, 235.0f),
			glm::vec3(0.2f, 0.2f, 0.2f),
			glm::vec3(0.0f, 2.0f, 2.0f),
			glm::vec3(1.0f, 1.0f, 1.0f),
			glm::vec3(1.0f, 0.01f, 0.002f)
		);
	}

	auto waterFBOs = Waters::WaterFrameBuffers();
	auto shadowFBO = Renderer::ShadowFrameBuffer();

	std::vector<GUI> guis;
	{
		guis.emplace_back
		(
			shadowFBO.buffer->depthTexture,
			glm::vec2(-0.5f, 0.5f),
			glm::vec2(0.5f, 0.5f)
		);

		/* guis.emplace_back
		(
			waterFBOs.refractionFBO->colorTexture,
			glm::vec2(-0.5f, -0.5f),
			glm::vec2(0.5f, 0.5f)
		); */
	}

	std::vector<WaterTile> waters;
	{
		waters.emplace_back
		(
			Resources::GetTexture("gfx/waterDUDV.png"),
			Resources::GetTexture("gfx/normal.png"),
			glm::vec3(120.0f, 3.7f, -2.0f)
		);
	}

	Entities::Camera camera(&player);
	Renderer::MasterRenderer renderer;

	startTime = frameStartTime = steady_clock::now();

	while (true)
	{
		// Prepare ImGUI
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplSDL2_NewFrame(window);
		ImGui::NewFrame();

		// Update
		ImGuiDisplay(lights);
		player.Move();
		camera.Move();

		// Begin render
		renderer.BeginFrame(entities, lights, player);
		// Draw shadow fbo
		renderer.RenderShadows(shadowFBO, camera);
		// Draw water framebuffers
		DrawWaterFBOs(waterFBOs, waters, renderer, camera);

		// Main render pass
		waterFBOs.BindDefaultFBO();
		renderer.RenderScene(camera);
		renderer.RenderWaters(waters, waterFBOs);
		renderer.RenderGUIs(guis);

		// End render
		renderer.EndFrame();

		// ImGUI render pass
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		SDL_GL_SwapWindow(window);
		CalculateFPS();
		if (PollEvents()) break;
	}
}

void SDLWindow::DrawWaterFBOs
(
	const WaterFrameBuffers& waterFBOs,
	const std::vector<WaterTile>& waters,
	MasterRenderer& renderer,
	Camera& camera
)
{
	// Enable clip plane 0
	glEnable(GL_CLIP_DISTANCE0);

	// Reflection pass
	waterFBOs.BindReflection();
	// Move the camera
	f32 distance = 2.0f * (camera.position.y - waters[0].position.y);
	camera.position.y -= distance;
	camera.InvertPitch();
	renderer.RenderScene(camera, glm::vec4(0.0f, 1.0f, 0.0f, -waters[0].position.y), Mode::Fast);
	// Move it back to its original position
	camera.position.y += distance;
	camera.InvertPitch();

	// Refraction pass
	waterFBOs.BindRefraction();
	renderer.RenderScene(camera, glm::vec4(0.0f, -1.0f, 0.0f, waters[0].position.y), Mode::Fast);

	// Disable clip plane 0
	glDisable(GL_CLIP_DISTANCE0);
}

void SDLWindow::CalculateFPS()
{
	endTime       = steady_clock::now();
	auto duration = duration_cast<milliseconds>(endTime - frameStartTime);
	g_Delta       = static_cast<f32>(static_cast<f64>(duration.count()) / 1000.0);

	frameStartTime = endTime;
	if (endTime - startTime >= seconds(1))
	{
		startTime = endTime;
		finalFPS  = FPS;
		frameTime = static_cast<f32>(1000.0 / FPS);
		FPS       = 0.0f;
	}
	++FPS;
}

void SDLWindow::ImGuiDisplay(std::vector<Light>& lights)
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

		if (ImGui::BeginMenu("Editor"))
		{
			if (ImGui::BeginMenu("Lights"))
			{
				static int current = 0;
				static const char* const items[] =
				{
					"[0]",
					"[1]",
					"[2]",
					"[3]"
				};

				ImGui::Combo("Current", &current, items, IM_ARRAYSIZE(items));
				ImGui::InputFloat3("Position",    &lights[current].position[0],    "%.1f");
				ImGui::InputFloat3("Ambient",     &lights[current].ambient[0],     "%.1f");
				ImGui::InputFloat3("Diffuse",     &lights[current].diffuse[0],     "%.1f");
				ImGui::InputFloat3("Specular",    &lights[current].specular[0],    "%.1f");
				ImGui::InputFloat3("Attenuation", &lights[current].attenuation[0], "%.1f");
				ImGui::EndMenu();
			}

			ImGui::EndMenu();
		}

		ImGui::EndMainMenuBar();
	}
	ImGuiUpdate();
}

void SDLWindow::ImGuiUpdate() const
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