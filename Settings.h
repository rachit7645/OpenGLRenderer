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
		// Window
		struct Window
		{
			// Size
			glm::ivec2 dimensions;
		} window = {};
		// Shadows
		struct Shadows
		{
			// Shadow map size
			glm::ivec2 dimensions;
			// Extra offset
			f32 offset;
		} shadows = {};
		// IBL
		struct IBL
		{
			// Environment map path
			std::string envMapPath;
		} ibl = {};
		// Player
		struct Player
		{
			f32 runSpeed;
			f32 turnSpeed;
		} player = {};
		// Camera
		struct Camera
		{
			// Zoom speed
			f32 zoomSpeed;
			// Pitch speed
			f32 pitchSpeed;
			// AAP speed
			f32 aapSpeed;
			// Min pitch
			f32 minPitch;
			// Max pitch
			f32 maxPitch;
		} camera = {};
	private:
		// Main constructor
		Settings();
	};
}

#endif