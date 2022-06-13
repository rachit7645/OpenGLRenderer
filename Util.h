#ifndef UTIL_H
#define UTIL_H

// Utility header file
// Contains utility macros and defines

#include <random>

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

// Handy global variables
inline const u8* g_Keys;
inline f32 g_Delta = 1.0f;

namespace Util
{
	// Random number between a range
	template<typename T>
	T Rand_Range(T min, T max)
	{
		static thread_local std::mt19937_64 generator(777);
		std::uniform_real_distribution<T> distributer(min, max);
		return distributer(generator);
	}

	// Truely random number between a range
	template<typename T>
	T True_Rand_Range(T min, T max)
	{
		std::random_device rd;
		std::seed_seq ss{ rd(), rd() };
		static thread_local std::mt19937_64 generator(ss);
		std::uniform_real_distribution<T> distributer(min, max);
		return distributer(generator);
	}

	// Clamp a number between two other numbers
	template<typename T>
	void Clamp(T& value, T min, T max)
	{
		if (value < min) value = min;
		if (value > max) value = max;
	}
}

#endif // UTIL_H