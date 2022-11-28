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
		auto window  = document["window"].get_object();

		// Parse window size
		auto windowX = window["width"].get_int64().value();
		auto windowY = window["height"].get_int64().value();
		// Set windowDimensions
		windowDimensions = glm::ivec2(static_cast<s32>(windowX), static_cast<s32>(windowY));

		// Parse gl object
		auto gl = document["gl"].get_object();

		// Parse glColor
		auto glR = gl["red"].get_int64().value();
		auto glG = gl["green"].get_int64().value();
		auto glB = gl["blue"].get_int64().value();
		auto glA = gl["alpha"].get_int64().value();
		// Set glColor
		glColor = glm::ivec4
		(
			static_cast<s32>(glR),
			static_cast<s32>(glG),
			static_cast<s32>(glB),
			static_cast<s32>(glA)
		);

		// Parse window glDepth
		auto glD = gl["depth"].get_int64().value();
		// Set glDepth
		glDepth = static_cast<s32>(glD);

		// Parse resources object
		auto resources = document["resources"].get_object();

		// Parse resources directory
		resourcesPath = resources["path"].get_string().value();

		// Parse camera object
		auto camera = document["camera"].get_object();

		// Parse camera speeds
		auto cameraZoom  = camera["zoom_speed"].get_double().value();
		auto cameraPitch = camera["pitch_speed"].get_double().value();
		auto cameraAAP   = camera["aap_speed"].get_double().value();
		// Set camera speeds
		cameraZoomSpeed  = static_cast<f32>(cameraZoom);
		cameraPitchSpeed = static_cast<f32>(cameraPitch);
		cameraAAPSpeed   = static_cast<f32>(cameraAAP);

		// Parse camera pitch bounds
		auto cameraMin = camera["pitch_min"].get_double().value();
		auto cameraMax = camera["pitch_max"].get_double().value();
		// Set camera pitch bounds
		cameraMinPitch = static_cast<f32>(cameraMin);
		cameraMaxPitch = static_cast<f32>(cameraMax);
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