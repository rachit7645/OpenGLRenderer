#ifndef UTIL_H
#define UTIL_H

// Utility header file
// Contains utility macros and typedefs

#include <cstdint>
#include <cstddef>

#define UNUSED __attribute__((unused))

typedef std::int8_t s8;
typedef std::int16_t s16;
typedef std::int32_t s32;
typedef std::int64_t s64;

typedef std::uint8_t u8;
typedef std::uint16_t u16;
typedef std::uint32_t u32;
typedef std::uint64_t u64;

typedef float f32;
typedef	double f64;

#define clamp(number, range) if (number > range) number = range


#endif