#include "Time.h"

#include <chrono>
#include <ctime>
#include <vector>

using std::chrono::system_clock;
using std::chrono::milliseconds;
using std::chrono::duration_cast;

// TODO: Add ms to time
// auto ms = duration_cast<milliseconds>(now.time_since_epoch()) % 1000;

std::string Util::GetTime()
{
	// Get current time, store ms, and convert to broken time
	auto now    = system_clock::now();
	auto timer  = system_clock::to_time_t(now);
	std::tm* bt = std::localtime(&timer);

	// Format HH:MM:SS with strftime
	std::vector<char> buffer(128);
	std::strftime(buffer.data(), buffer.size() * sizeof(char), "[%H:%M:%S]", bt);
	return buffer.data();
}