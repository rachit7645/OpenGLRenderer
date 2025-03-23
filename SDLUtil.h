#ifndef SDL_UTIL_H
#define SDL_UTIL_H

#include <SDL2/SDL.h>

#include "Util.h"

namespace SDL
{
    void SetGLAttribute(SDL_GLattr attribute, s32 value);
    void CheckGLAttribute(SDL_GLattr attribute, s32 value);
}

#endif
