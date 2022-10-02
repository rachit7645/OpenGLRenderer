#ifndef RANDOM_H
#define RANDOM_H

#include <random>

namespace Util
{
	// Random number between a range
	template<typename T>
	T RandRange(T min, T max)
	{
		static thread_local std::mt19937_64 generator(777);
		std::uniform_real_distribution<T> distributer(min, max);
		return distributer(generator);
	}

	// Truly random number between a range
	template<typename T>
	T TrueRandRange(T min, T max)
	{
		std::random_device rd;
		std::seed_seq ss{ rd(), rd() };
		static thread_local std::mt19937_64 generator(ss);
		std::uniform_real_distribution<T> distributer(min, max);
		return distributer(generator);
	}
}

#endif