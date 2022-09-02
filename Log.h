#ifndef LOG_H
#define LOG_H

#include <iostream>
#include <cstdlib>
#include <fmt/format.h>

#include "Util.h"
#include "Files.h"
#include "Time.h"

namespace Logger
{
	template<typename... Args>
	void Log
	(
		const std::string& type,
		const std::string& time,
		const std::string& file,
		int line,
		fmt::string_view format,
		Args&&... args
	)
	{
		fmt::print(stderr, "[{}] [{}] [{}:{}] ", type, time, file, line);
		fmt::print(stderr, format, args...);
	}
}

// Logging macros

#define LOG_INFO(format, ...) \
	do \
	{ \
		Logger::Log("INFO", Util::GetTime(), Files::GetName(__FILE__), __LINE__, FMT_STRING(format), __VA_ARGS__); \
	} \
	while (0)

#define LOG_DEBUG(...) /*\
	do \
	{ \
		Logger::Log("[DEBUG] [", Util::GetTime(), "] [", Files::GetName(__FILE__), ":", __LINE__, "] ", __VA_ARGS__); \
	} \
	while (0)*/

#define LOG_WARNING(...) /*\
	do \
	{ \
		 Logger::Log("[WARNING] [", Util::GetTime(), "] [", Files::GetName(__FILE__), ":", __LINE__, "] ", __VA_ARGS__); \
	} \
	while (0)*/

#define LOG_ERROR(...) /*\
	do \
	{ \
		Logger::Log("[ERROR] [", Util::GetTime(), "] [", Files::GetName(__FILE__), ":", __LINE__, "] ", __VA_ARGS__); \
		std::exit(-1); \
	} \
	while (0)*/

#define LOG_GL(...) /*\
	do \
    { \
		Logger::Log("[GL] [", Util::GetTime(), "] ", __VA_ARGS__); \
	} \
	while(0)*/

#endif // LOG_H