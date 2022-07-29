#include "Time.h"

#include <chrono>
#include <ctime>

using std::chrono::system_clock;
using std::chrono::milliseconds;
using std::chrono::duration_cast;

// TODO: Add ms to time
// auto ms = duration_cast<milliseconds>(now.time_since_epoch()) % 1000;

constexpr auto TIME_BUFFER_LENGTH = 128;
constexpr auto TIME_BUFFER_SIZE   = TIME_BUFFER_LENGTH * sizeof(char);

std::string Util::GetTime()
{
	// Get current time, store ms, and convert to broken time
	auto now    = system_clock::now();
	auto timer  = system_clock::to_time_t(now);
	std::tm* bt = std::localtime(&timer);

	// Format HH:MM:SS with strftime
	char buffer[TIME_BUFFER_LENGTH];
	std::strftime(buffer, TIME_BUFFER_SIZE, "[%H:%M:%S]", bt);
	return buffer;
}