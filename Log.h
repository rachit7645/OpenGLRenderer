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
	constexpr auto INFO_STR = "INFO: ", WARNING_STR = "WARNING: ", ERROR_STR = "ERROR: ";

	// Log message with SDL error and exit with specified exit code
	void LogAndExit_SDL(const std::string& message, int exitCode);
	// Log message and exit with specified exit code
	void LogAndExit(const std::string& message, int exitCode);
	// Log message with formatting
	void Log(const std::string& message, Type type);

}

#endif // LOG_H