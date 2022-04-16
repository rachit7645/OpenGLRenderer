#include "Log.h"

void Logger::LogAndExit_SDL(const std::string& message, Error error)
{
	Log(message + SDL_GetError(), ERROR);
	std::exit(error);
}

void Logger::LogAndExit(const std::string& message, Error error)
{
	Log(message, ERROR);
	std::exit(error);
}

void Logger::Log(const std::string& message, LogType type)
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