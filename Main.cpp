#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>

#include "Instance.h"
#include "Util.h"

// TODO: Make engine restartable

int main(UNUSED int argc, UNUSED char** argv)
{
    // Create instance
    auto instance = Engine::Instance();
    // Start instance
    instance.Run();
    // Return
    return 0;
}