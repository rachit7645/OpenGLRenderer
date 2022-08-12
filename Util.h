#ifndef UTIL_H
#define UTIL_H

// Utility header file
// Contains utility macros and defines

#include <random>
#include <vector>

#include <cstdint>
#include <cstddef>
#include <cassert>

#ifdef __CLION_IDE__
#define UNUSED [[maybe_unused]]
#else
#define UNUSED __attribute__((unused))
#endif

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

	// Truly random number between a range
	template<typename T>
	T True_Rand_Range(T min, T max)
	{
		std::random_device rd;
		std::seed_seq ss{ rd(), rd() };
		static thread_local std::mt19937_64 generator(ss);
		std::uniform_real_distribution<T> distributer(min, max);
		return distributer(generator);
	}

	// TODO: Write a custom 2D array wrapper
	// std::vector<std::vector<T>> is not contiguous in layout
	// This wrecks havoc with the cpu cache
	template<typename T>
	using Array2D = std::vector<std::vector<T>>;

	template<typename T>
	constexpr void InitArray2D(Array2D<T>& array, size_t size)
	{
		array = Array2D<T>(size);
		for (auto& row : array)
		{
			row = std::vector<T>(size);
		}
	}
}

#endif // UTIL_H