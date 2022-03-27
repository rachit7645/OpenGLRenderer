#include "Log.h"

// Log message with SDL error and exit with specified exit code
void Logger::LogAndExit_SDL(std::string message, int exitCode)
{
	std::string SDLError(SDL_GetError());
	Log(message + SDLError, ERROR);
	std::exit(exitCode);
}

//Log message and exit with specified exit code
void Logger::LogAndExit(std::string message, int exitCode)
{
	Log(message, ERROR);
	std::exit(exitCode);
}

// Log message with formatting
void Logger::Log(std::string message, Type type)
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