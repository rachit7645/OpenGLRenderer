#ifndef LOG_H
#define LOG_H

#include <string>
#include <cstdlib>
#include <iostream>
#include <SDL2/SDL.h>

namespace Logger
{
	enum Type
	{
		INFO = 0,
		WARNING = 1,
		ERROR = 2,
	};
	constexpr const char* INFO_STR = "INFO: ", *WARNING_STR = "WARNING: ", *ERROR_STR = "ERROR: ";

	void LogAndExit_SDL(std::string message, int exitCode);
	void LogAndExit(std::string message, int exitCode);
	void Log(std::string message, Type type);

}

#endif // LOG_H