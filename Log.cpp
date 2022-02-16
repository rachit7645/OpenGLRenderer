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
	std::string m_type;

	if (type == INFO) 
		m_type = "INFO: "; 
	else if (type == WARNING) 
		m_type = "WARNING: ";
	else if (type == ERROR)
		m_type = "ERROR: ";

	std::cerr << m_type << message;	
	 
}