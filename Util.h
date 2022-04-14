#ifndef UTIL_H
#define UTIL_H

// Utility header file
// Contains utility macros and defines

#include <cstdint>
#include <cstddef>
#include <cassert>

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
	ASSIMP_LOAD_FAILED = 7,
};

template<typename T>
T Rand_Range(T min, T max)
{
	return min + static_cast<T> (std::rand() / static_cast<T>(RAND_MAX / ( max - min )));
} 

#endif // UTIL_H