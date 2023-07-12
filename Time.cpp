#include "Time.h"

#include <chrono>
#include <fmt/format.h>
#include <fmt/chrono.h>

using std::chrono::system_clock;

std::string Util::GetTime()
{
    return fmt::format("{:%H:%M:%S}", system_clock::now());
}