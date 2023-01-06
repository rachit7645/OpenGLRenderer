#ifndef LOG_H
#define LOG_H

#include <string>
#include <string_view>
#include <cstdlib>
#include <fmt/format.h>

#include "Files.h"
#include "Time.h"

namespace Logger
{
	template<typename... Args>
	void Log
	(
		const std::string_view type,
		const std::string_view time,
		const std::string_view file,
		int line,
		fmt::string_view format,
		Args&&... args
	)
	{
		fmt::print(stderr, "[{}] [{}] [{}:{}] ", type, time, file, line);
		fmt::print(stderr, format, args...);
	}
}

// Implementation

#define IMPL_LOG(type, format, ...) \
	do \
	{ \
		Logger::Log(type, Util::GetTime(), Engine::Files::GetInstance().GetName(__FILE__), __LINE__, FMT_STRING(format), __VA_ARGS__); \
	} \
	while (0)

#define IMPL_LOG_EXIT(type, format, ...) \
	do \
	{ \
		IMPL_LOG(type, format, __VA_ARGS__); \
    	std::exit(-1); \
	} \
	while (0)

// Regular loggers

#define LOG_INFO(format, ...)    IMPL_LOG("INFO",    format, __VA_ARGS__)
#define LOG_DEBUG(format, ...)   IMPL_LOG("DEBUG",   format, __VA_ARGS__)
#define LOG_WARNING(format, ...) IMPL_LOG("WARNING", format, __VA_ARGS__)
#define LOG_GL(format, ...)      IMPL_LOG("GL",      format, __VA_ARGS__)

// Error loggers

#define LOG_ERROR(format, ...) IMPL_LOG_EXIT("ERROR", format, __VA_ARGS__)

#endif