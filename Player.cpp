#include "Player.h"

#include "Inputs.h"
#include "Settings.h"

// Using namespaces
using namespace Entities;

// Usings
using Renderer::Model;
using Renderer::MeshTextures;
using Engine::Settings;
using Engine::Inputs;

Player::Player()
    : Entity(nullptr, Transform())
{
}

Player::Player(MdPtr model, const Transform& transform)
    : Entity(std::move(model), transform)
{
}

void Player::Move()
{
    // Check keyboard input
    CheckInputs();

    // Change rotation
    transform.rotation.y  += m_turnSpeed * Util::g_Delta;

    // Calculate distance
    f32 distance = m_runSpeed * Util::g_Delta;
    // Change position
    transform.position.x  += distance * std::sin(glm::radians(transform.rotation.y));
    transform.position.z  += distance * std::cos(glm::radians(transform.rotation.y));

    // Set speeds to 0
    m_turnSpeed = 0.0f;
    m_runSpeed  = 0.0f;

    // Display ImGui widgets
    ImGuiDisplay();
}

void Player::ImGuiDisplay()
{
    // Begin main menu
    if (ImGui::BeginMainMenuBar())
    {
        // Begin player menu
        if (ImGui::BeginMenu("Player"))
        {
            // Player position
            ImGui::Text("Position:");
            ImGui::InputFloat3("##pos", &transform.position[0], "%.1f");
            // Player rotation
            ImGui::Text("Rotation:");
            ImGui::InputFloat3("##rot", &transform.rotation[0], "%.1f");
            // Player scale
            ImGui::Text("Scale:");
            ImGui::InputFloat3("##scl", &transform.scale[0], "%.1f");
            // End menu
            ImGui::EndMenu();
        }

        // End main menu bar
        ImGui::EndMainMenuBar();
    }
}

void Player::CheckInputs()
{
    // Get settings
    const auto& settings = Settings::GetInstance();
    // Get inputs
    auto& inputs = Inputs::GetInstance();

    // W Key
    if (inputs.IsKeyPressed(SDL_SCANCODE_W))
    {
        // Forward direction
        m_runSpeed = settings.player.runSpeed;
    }
    // S Key
    else if (inputs.IsKeyPressed(SDL_SCANCODE_S))
    {
        // Backward direction
        m_runSpeed = -settings.player.runSpeed;
    }

    // A Key
    if (inputs.IsKeyPressed(SDL_SCANCODE_A))
    {
        // Clockwise turn
        m_turnSpeed = settings.player.turnSpeed;
    }
    // D Key
    else if (inputs.IsKeyPressed(SDL_SCANCODE_D))
    {
        // Anti-clockwise turn
        m_turnSpeed = -settings.player.turnSpeed;
    }
}