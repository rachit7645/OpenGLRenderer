#ifndef UTIL_H
#define UTIL_H

// Utility header file
// Contains utility macros and defines

#include <cstdint>
#include <cstddef>
#include <cassert>
#include <cmath>

#ifdef __CLION_IDE__
#define UNUSED [[maybe_unused]]
#else
#define UNUSED __attribute__((unused))
#endif

#if _WIN32
#define EXPORT __declspec(dllexport)
#else
#define EXPORT
#endif

#define STRINGIFY_HELPER(x) #x
#define STRINGIFY(x) STRINGIFY_HELPER(x)

using s8  = std::int8_t;
using s16 = std::int16_t;
using s32 = std::int32_t;
using s64 = std::int64_t;

using u8  = std::uint8_t;
using u16 = std::uint16_t;
using u32 = std::uint32_t;
using u64 = std::uint64_t;

using f32 = float;
using f64 = double;

using ssize = ssize_t;
using usize = std::size_t;

// Handy global variables
inline f32 g_Delta = 1.0f;

#endif