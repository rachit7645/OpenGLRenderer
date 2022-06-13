#ifndef LOG_H
#define LOG_H

#include <iostream>
#include <cstdlib>

#include "Util.h"
#include "Files.h"

namespace Logger
{
	template<typename... Args>
	void Log(Args... args)
	{
		(std::cerr << ... << args);
	}
}

// Logging macros
// TODO: Add time to logs

#define LOG_INFO(...)    do { Logger::Log("[INFO] [",    Files::GetFileName(__FILE__), ":", __LINE__, "] ", __VA_ARGS__); }   while (0)
#define LOG_DEBUG(...)   do { Logger::Log("[DEBUG] [",   Files::GetFileName(__FILE__), ":", __LINE__, "] ", __VA_ARGS__); }   while (0)
#define LOG_WARNING(...) do { Logger::Log("[WARNING] [", Files::GetFileName(__FILE__), ":", __LINE__, "] ", __VA_ARGS__); }   while (0)
#define LOG_ERROR(...)   do { Logger::Log("[ERROR] [",   Files::GetFileName(__FILE__), ":", __LINE__, "] ", __VA_ARGS__); std::exit(-1); } while (0)

#endif // LOG_H