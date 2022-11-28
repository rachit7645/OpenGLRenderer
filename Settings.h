#ifndef SETTINGS_H
#define SETTINGS_H

#include "GLM.h"
#include "Util.h"

namespace Engine
{
	// Settings class
	class Settings
	{
	public:
		// Singleton getter
		static const Settings& GetInstance();
		// Window windowDimensions
		glm::ivec2 windowDimensions = {0, 0};
		// Color bits
		glm::ivec4 glColor = {0, 0, 0, 0};
		// Depth bits
		s32 glDepth = 0;
		// Resource directory
		std::string resourcesPath;
		// Camera zoom speed
		f32 cameraZoomSpeed = 0.0f;
		// Camera pitch speed
		f32 cameraPitchSpeed = 0.0f;
		// Camera aap speed
		f32 cameraAAPSpeed = 0.0f;
		// Camera min pitch
		f32 cameraMinPitch = 0.0f;
		// Camera max pitch
		f32 cameraMaxPitch = 0.0f;
	private:
		// Main constructor
		Settings();
	};
}

#endif