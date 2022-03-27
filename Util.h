#ifndef UTIL_H
#define UTIL_H

// Utility header file
// Contains utility macros and usings

#include <cstdint>
#include <cstddef>

#define UNUSED __attribute__((unused))

using s8 = std::int8_t;
using s16 = std::int16_t;
using s32 = std::int32_t;
using s64 = std::int64_t;

using u8 = std::uint8_t;
using u16 = std::uint16_t;
using u32 = std::uint32_t;
using u64 = std::uint64_t;

using f32 = float;
using f64 = double;

enum ERRORS
{
	SDL_INIT_FAILED = 1,
	SDL_CREATE_WINDOW_FAILED = 2,
	SDL_GL_CREATE_CONTEXT_FAILED = 3,
	GLEW_INIT_FAILED = 4,
	SHADER_FILE_OPEN_FAILED = 5,
	SHADER_COMPILATION_FAILED = 6,
};

#endif // UTIL_H