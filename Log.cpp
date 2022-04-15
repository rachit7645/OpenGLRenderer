#include "Log.h"

void Logger::LogAndExit_SDL(const std::string& message, int exitCode)
{
	std::string SDLError = SDL_GetError();
	Log(message + SDLError, ERROR);
	std::exit(exitCode);
}

void Logger::LogAndExit(const std::string& message, int exitCode)
{
	Log(message, ERROR);
	std::exit(exitCode);
}

void Logger::Log(const std::string& message, Type type)
{
	const char* str_type;
	if (type == INFO) 
		str_type = INFO_STR;
	else if (type == WARNING)
		str_type = WARNING_STR;
	else if (type == ERROR)
		str_type = ERROR_STR;

	std::cerr << str_type << message;
}