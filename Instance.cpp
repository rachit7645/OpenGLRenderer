#include "Instance.h"

#include "Resources.h"
#include "MeshTextures.h"
#include "LightsBuffer.h"

// Using namespaces
using namespace Engine;

// Namespace aliases
namespace chrono = std::chrono;

// Usings
using Renderer::MeshTextures;

Instance::Instance()
{
    // Initialise data
    InitEntities();
    InitLights();
    InitMisc();
}

void Instance::Run()
{
    // Set start time
    m_startTime = m_frameStartTime = chrono::steady_clock::now();

    while (true)
    {
        // Prepare ImGui
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplSDL2_NewFrame(m_window.handle);
        ImGui::NewFrame();

        // Update
        ImGuiDisplay();
        m_player.Move();
        m_camera.Move();

        // Begin render
        m_renderer.BeginFrame(m_entities, m_dirLights, m_pointLights, m_spotLights, m_player);

        // Draw shadow map
        m_renderer.RenderShadows(m_camera, m_dirLights[0].position);

        // Get time (t ∈ [-π, π])
        auto time = std::acos(cos(static_cast<f32>(SDL_GetTicks64()) / 1000.0f));
        // Change positions
        m_pointLights[0].position.x = static_cast<f32>(std::cos(1.0 * time) * 25.0);
        m_pointLights[0].position.z = static_cast<f32>(std::cos(2.0 * time) * 7.0);
        // Draw point shadow map
        m_renderer.RenderPointShadows(m_pointLights);

        // Draw spot shadow map
        m_renderer.RenderSpotShadows(m_spotLights);

        // Draw water framebuffers
        m_renderer.RenderWaterFBOs(m_waters, m_camera);

        // Deferred geometry pass
        m_renderer.RenderGBuffer(m_camera);
        // Deferred lighting pass
        m_renderer.RenderLighting(m_camera);

        // Render waters
        m_renderer.RenderWaters(m_waters);
        // Render skybox
        m_renderer.RenderSkybox();

        // Bloom passes
        m_renderer.RenderBloom();
        // Post-processing pass
        m_renderer.RenderPostProcess();

        // End render
        m_renderer.EndFrame();

        // ImGui render pass
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        // Swap handle
        SDL_GL_SwapWindow(m_window.handle);
        // Calculate FPS
        CalculateFPS();

        // Poll events
        if (m_window.PollEvents()) break;
    }
}

void Instance::CalculateFPS()
{
    // Calculate end time
    m_endTime = chrono::steady_clock::now();
    // Calculate frame duration
    auto duration = chrono::duration_cast<chrono::milliseconds>(m_endTime - m_frameStartTime);
    // Calculate cycle duration
    auto cycleDuration = m_endTime - m_startTime;
    // Calculate frame delta
    Util::g_Delta = static_cast<f32>(static_cast<f64>(duration.count()) / 1000.0);
    // Set this/next frame's start time
    m_frameStartTime = m_endTime;

    // If a second has passed
    if (cycleDuration >= chrono::seconds(1))
    {
        // Set this cycle's start time
        m_startTime = m_endTime;
        // Update displayed FPS
        m_finalFPS  = m_FPS;
        // Calculate frame time
        m_frameTime = static_cast<f32>(1000.0 / m_FPS);
        // Reset FPS
        m_FPS = 0.0f;
    }

    // Increment FPS
    ++m_FPS;
}

void Instance::ImGuiDisplay()
{
    // If menu bar is visible
    if (ImGui::BeginMainMenuBar())
    {
        // If options menu is selected
        if (ImGui::BeginMenu("Options"))
        {
            // Display m_FPS info
            ImGui::Text("FPS: %.2f", m_finalFPS);
            ImGui::Text("Frame time: %.2f ms", m_frameTime);
            // Display options
            ImGui::Checkbox("Vsync", &m_vsync);
            ImGui::Checkbox("Wireframe", &m_wireframe);
            // End menu
            ImGui::EndMenu();
        }

        // If editor menu is selected
        if (ImGui::BeginMenu("Editor"))
        {
            // If lights menu is selected
            if (ImGui::BeginMenu("Lights"))
            {
                // Shared index item getter lambda
                auto GetItem = [] (void* data, int index, const char** outText) -> bool
                {
                    // Get vector
                    auto& vector = *reinterpret_cast<std::vector<std::string>*>(data);
                    // Put data
                    *outText = vector[index].data();
                    // Return success
                    return true;
                };

                // If directional lights menu is selected
                if (ImGui::BeginMenu("Directional"))
                {
                    // Generate indices
                    auto indices = GenerateIndices(m_dirLights.size());
                    // Light selector
                    ImGui::Combo
                    (
                        "Slot",                            // Displayed name
                        &m_selectedDirLight,               // Selected light
                        GetItem,                           // Item Getter
                        reinterpret_cast<void*>(&indices), // Raw data
                        static_cast<s32>(indices.size())   // Data size
                    );

                    // Select light
                    auto& light = m_dirLights[m_selectedDirLight];
                    // Position
                    ImGui::DragFloat3("Position", &light.position[0], 0.5f, -500.0f, 500.0f, "%.1f");
                    // Color
                    ImGui::ColorEdit3("Color", &light.color[0]);
                    // Intensity
                    ImGui::DragFloat3("Intensity", &light.intensity[0], 0.5f, 0.0f, 100.0f, "%.1f");

                    // If new light needs to be added
                    if (ImGui::Button("Add"))
                    {
                        // If light count is less than max
                        if (m_dirLights.size() < Renderer::SHADER_MAX_LIGHTS)
                        {
                            // Create new empty light
                            m_dirLights.emplace_back();
                        }
                    }

                    // Put both buttons on the same line
                    ImGui::SameLine();

                    // If selected light needs to be deleted
                    if (ImGui::Button("Delete"))
                    {
                        // If light count is more than 1 (Light #1 is shadow casting)
                        if (m_dirLights.size() > 1)
                        {
                            // Delete light
                            m_dirLights.erase(m_dirLights.begin() + m_selectedDirLight);
                            // Change displayed light index
                            if (m_selectedDirLight > 0) --m_selectedDirLight;
                        }
                    }

                    // End menu
                    ImGui::EndMenu();
                }

                // If point lights menu is selected
                if (ImGui::BeginMenu("Point"))
                {
                    // Generate indices
                    auto indices = GenerateIndices(m_pointLights.size());
                    // Light selector
                    ImGui::Combo
                    (
                        "Slot",                            // Displayed name
                        &m_selectedPointLight,             // Selected light
                        GetItem,                           // Item Getter
                        reinterpret_cast<void*>(&indices), // Raw data
                        static_cast<s32>(indices.size())   // Data size
                    );

                    // Select light
                    auto& light = m_pointLights[m_selectedPointLight];
                    // Position
                    ImGui::DragFloat3("Position", &light.position[0], 0.5f, -500.0f, 500.0f, "%.1f");
                    // Color
                    ImGui::ColorEdit3("Color", &light.color[0]);
                    // Intensity
                    ImGui::DragFloat3("Intensity", &light.intensity[0], 0.5f, 0.0f, 100.0f, "%.1f");
                    // Attenuation
                    ImGui::InputFloat3("Attenuation", &light.attenuation[0], "%.4f");

                    // If new light needs to be added
                    if (ImGui::Button("Add"))
                    {
                        // If light count is less than max
                        if (m_pointLights.size() < Renderer::SHADER_MAX_LIGHTS)
                        {
                            // Create new empty light
                            m_pointLights.emplace_back();
                        }
                    }

                    // Put both buttons on the same line
                    ImGui::SameLine();

                    // If selected light needs to be deleted
                    if (ImGui::Button("Delete"))
                    {
                        // If light count is not zero
                        if (!m_pointLights.empty())
                        {
                            // Delete light
                            m_pointLights.erase(m_pointLights.begin() + m_selectedPointLight);
                            // Change displayed light index
                            if (m_selectedPointLight > 0) --m_selectedPointLight;
                        }
                    }

                    // End menu
                    ImGui::EndMenu();
                }

                // If spot lights menu is selected
                if (ImGui::BeginMenu("Spot"))
                {
                    // Generate indices
                    auto indices = GenerateIndices(m_spotLights.size());
                    // Light selector
                    ImGui::Combo
                    (
                        "Slot",                            // Displayed name
                        &m_selectedSpotLight,              // Selected light
                        GetItem,                           // Item Getter
                        reinterpret_cast<void*>(&indices), // Raw data
                        static_cast<s32>(indices.size())   // Data size
                    );

                    // Select light
                    auto& light = m_spotLights[m_selectedSpotLight];
                    // Position
                    ImGui::DragFloat3("Position", &light.position[0], 0.5f, -500.0f, 500.0f, "%.1f");
                    // Color
                    ImGui::ColorEdit3("Color", &light.color[0]);
                    // Intensity
                    ImGui::DragFloat3("Intensity", &light.intensity[0], 0.5f, 0.0f, 100.0f, "%.1f");
                    // Attenuation
                    ImGui::InputFloat3("Attenuation", &light.attenuation[0], "%.4f");
                    // Direction
                    ImGui::DragFloat3("Direction", &light.direction[0], 0.05f, -1.0f, 1.0f, "%.1f");
                    // Get cut off in degrees
                    auto degCutOff = light.GetCutOff();
                    // Cut off
                    ImGui::DragFloat2("CutOff", &degCutOff[0], 0.5f, 0.0f, 180.0f, "%.1f");
                    // Set new cut off
                    light.SetCutOff(degCutOff);

                    // If new light needs to be added
                    if (ImGui::Button("Add"))
                    {
                        // If light count is less than max
                        if (m_spotLights.size() < Renderer::SHADER_MAX_LIGHTS)
                        {
                            // Create new empty light
                            m_spotLights.emplace_back();
                        }
                    }

                    // Put both buttons on the same line
                    ImGui::SameLine();

                    // If selected light needs to be deleted
                    if (ImGui::Button("Delete"))
                    {
                        // If light count is not zero
                        if (!m_spotLights.empty())
                        {
                            // Delete light
                            m_spotLights.erase(m_spotLights.begin() + m_selectedSpotLight);
                            // Change displayed light index
                            if (m_selectedSpotLight > 0) --m_selectedSpotLight;
                        }
                    }

                    // End menu
                    ImGui::EndMenu();
                }

                // End menu
                ImGui::EndMenu();
            }

            // End menu
            ImGui::EndMenu();
        }

        // End menu bar
        ImGui::EndMainMenuBar();
    }

    ImGuiUpdate();
}

void Instance::ImGuiUpdate()
{
    // Calculate selections
    GLenum selectedPolyMode = m_wireframe ? GL_LINE : GL_FILL;
    s32    selectedSwapMode = m_vsync ? -1 : 0;

    // Wireframe check
    if (selectedPolyMode != m_currentPolyMode)
    {
        glPolygonMode(GL_FRONT_AND_BACK, selectedPolyMode);
        m_currentPolyMode = selectedPolyMode;
    }

    // V-SYNC check
    if (selectedSwapMode != m_currentSwapMode)
    {
        SDL_GL_SetSwapInterval(selectedSwapMode);
        m_currentSwapMode = selectedSwapMode;
    }
}

std::vector<std::string> Instance::GenerateIndices(usize size)
{
    // Create vector
    std::vector<std::string> indices;

    // For each element
    for (usize i = 0; i < size; ++i)
    {
        // Create index
        indices.emplace_back("[" + std::to_string(i) + "]");
    }

    // Return indices
    return indices;
}

void Instance::InitEntities()
{
    // Get resource handle
    auto& resources = Resources::GetInstance();

    // Default textures
    auto defaultTextures = MeshTextures
    (
        resources.GetTexture("gfx/def.png"),
        resources.GetTexture("gfx/defNrm.png"),
        resources.GetTexture("gfx/def.png"),
        resources.GetTexture("gfx/defEmm.png")
    );

    // All models
    auto playerModel = resources.GetModel("gfx/Mario/Mario.gltf",  defaultTextures);
    auto sponzaModel = resources.GetModel("gfx/Sponza/sponza.glb", defaultTextures);

    // Entities
    m_entities =
    {
        {
            sponzaModel,
            {
                glm::vec3(0.0f, 0.0f, 0.0f),
                glm::vec3(0.0f, 0.0f, 0.0f),
                glm::vec3(0.05f, 0.05f, 0.05f)
            }
        }
    };

    // Player
    m_player =
    {
        playerModel,
        {
            glm::vec3(0.0f, 0.0f, -4.0f),
            glm::vec3(0.0f, -90.0f, 0.0f),
            glm::vec3(0.03f, 0.03f, 0.03f)
        }
    };

    // Attach player
    m_camera.player = &m_player;
}

void Instance::InitLights()
{
    // Directional lights
    m_dirLights =
    {
        {
            glm::vec3(-16.0f, 100.0f, -10.0f),
            glm::vec3(0.3f, 0.3f, 0.3f),
            glm::vec3(1.0f, 1.0f, 1.0f)
        }
    };

    // Point lights
    m_pointLights =
    {
        {
            glm::vec3(4.0f, 20.0f, -3.0f),
            glm::vec3(0.0f, 0.945f, 0.945f),
            glm::vec3(1.0f, 7.0f, 5.0f),
            glm::vec3(1.0f, 0.022f, 0.0019f)
        },
        {
            glm::vec3(27.0f, 15.0f, -3.0f),
            glm::vec3(0.0f, 0.031f, 1.0f),
            glm::vec3(1.0f, 6.0f, 10.0f),
            glm::vec3(1.0f, 0.027f, 0.0028f)
        }
    };

    // Spot lights
    m_spotLights =
    {
        {
            glm::vec3(22.0f, 10.0f, 6.0f),
            glm::vec3(1.0f, 0.0f, 0.0f),
            glm::vec3(10.0f, 1.0f, 1.0f),
            glm::vec3(1.0f, 0.007f, 0.0002f),
            glm::vec3(-1.0f, 0.0f, -0.3f),
            glm::vec2(10.0f, 30.0f)
        },
        {
            glm::vec3(62.0f, 2.0f, -2.0f),
            glm::vec3(0.941f, 0.0f, 1.0f),
            glm::vec3(10.0f, 10.0f, 10.0f),
            glm::vec3(1.0f, 0.022f, 0.0019f),
            glm::vec3(-1.0f, 0.3f, -0.1f),
            glm::vec2(10.0f, 50.0f)
        }
    };
}

void Instance::InitMisc()
{
    // Get resource handle
    auto& resources = Resources::GetInstance();

    // Waters
    m_waters =
    {
        {
            resources.GetTexture("gfx/Water/waterDUDV.png"),
            resources.GetTexture("gfx/Water/normal.png"),
            {
                glm::vec3(120.0f, 3.7f, -2.0f),
                glm::vec3(0.0f, 0.0f, 0.0f),
                glm::vec3(Waters::WATER_TILE_SIZE, Waters::WATER_TILE_SIZE, Waters::WATER_TILE_SIZE)
            }
        }
    };
}