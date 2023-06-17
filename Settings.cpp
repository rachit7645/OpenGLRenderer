#include "Settings.h"

#include "JSON.h"
#include "Log.h"
#include "Files.h"

// Using namespaces
using namespace Engine;

// Namespace aliases
namespace ondemand = simdjson::ondemand;

// Usings
using simdjson::padded_string;

// Constants
constexpr auto SETTINGS_PATH = "../settings.json";

Settings::Settings()
{
    try
    {
        LOG_INFO("Loading settings: {}\n", SETTINGS_PATH);
        // Create parser
        ondemand::parser parser;
        // Load settings file
        auto json = padded_string::load(SETTINGS_PATH);
        // Create document
        ondemand::document document = parser.iterate(json);

        // Parse window object
        auto windowJSON  = document["window"].get_object();
        // Parse window size
        auto windowX = windowJSON["width"].get_int64().value();
        auto windowY = windowJSON["height"].get_int64().value();
        // Set window dimensions
        window.dimensions = glm::ivec2(static_cast<s32>(windowX), static_cast<s32>(windowY));

        // Parse shadows object
        auto shadowsJSON = document["shadows"].get_object();
        // Parse shadow map dimensions
        auto shadowX = shadowsJSON["width"].get_int64().value();
        auto shadowY = shadowsJSON["height"].get_int64().value();
        // Set shadow map size
        shadows.dimensions = glm::ivec2(static_cast<s32>(shadowX), static_cast<s32>(shadowY));
        // Parse shadow map offset
        auto shadowOffset = shadowsJSON["offset"].get_double().value();
        // Set shadow map offset
        shadows.offset = static_cast<f32>(shadowOffset);

        // Parse point shadows object
        auto pointShadowsJSON = document["point_shadows"].get_object();
        // Parse shadow map dimensions
        auto pointShadowX = pointShadowsJSON["width"].get_int64().value();
        auto pointShadowY = pointShadowsJSON["height"].get_int64().value();
        // Set shadow map size
        pointShadows.dimensions = glm::ivec2(static_cast<s32>(pointShadowX), static_cast<s32>(pointShadowY));

        // Parse water object
        auto waterJSON = document["water"].get_object();
        // Parse multipliers
        auto waterMultiplierX = waterJSON["width_multiplier"].get_double().value();
        auto waterMultiplierY = waterJSON["height_multiplier"].get_double().value();
        // Set water multipliers
        water.multipliers = glm::vec2(static_cast<f32>(waterMultiplierX), static_cast<f32>(waterMultiplierY));

        // Parse IBL object
        auto iblJSON = document["ibl"].get_object();
        // Parse environment map path
        auto envMapPath = iblJSON["env_map_path"].get_string().value();
        // Set environment map path
        ibl.envMapPath = envMapPath;

        // Parse player object
        auto playerJSON = document["player"].get_object();
        // Parse player speeds
        auto playerRunSpeed = playerJSON["run_speed"].get_double().value();
        auto playerTurnSpeed = playerJSON["turn_speed"].get_double().value();
        // Set player speeds
        player.runSpeed  = static_cast<f32>(playerRunSpeed);
        player.turnSpeed = static_cast<f32>(playerTurnSpeed);

        // Parse camera object
        auto cameraJSON = document["camera"].get_object();
        // Parse camera speeds
        auto cameraZoom  = cameraJSON["zoom_speed"].get_double().value();
        auto cameraPitch = cameraJSON["pitch_speed"].get_double().value();
        auto cameraAAP   = cameraJSON["aap_speed"].get_double().value();
        // Set camera speeds
        camera.zoomSpeed  = static_cast<f32>(cameraZoom);
        camera.pitchSpeed = static_cast<f32>(cameraPitch);
        camera.aapSpeed   = static_cast<f32>(cameraAAP);
        // Parse camera pitch bounds
        auto cameraMin = cameraJSON["pitch_min"].get_double().value();
        auto cameraMax = cameraJSON["pitch_max"].get_double().value();
        // Set camera pitch bounds
        camera.minPitch = static_cast<f32>(cameraMin);
        camera.maxPitch = static_cast<f32>(cameraMax);
    }
    catch (const std::exception& exception)
    {
        LOG_ERROR("Parsing settings failed! : {}", exception.what());
    }
}

const Settings& Settings::GetInstance()
{
    // Static storage
    static auto settings = Settings();
    // Return
    return settings;
}