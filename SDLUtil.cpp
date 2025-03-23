#include "SDLUtil.h"

#include "Log.h"

namespace SDL
{
    void SetGLAttribute(SDL_GLattr attribute, s32 value)
    {
        if (SDL_GL_SetAttribute(attribute, value) != 0)
        {
            LOG_ERROR("SDL_GL_SetAttribute Failed\n{}\n", SDL_GetError());
        }
    }

    void CheckGLAttribute(SDL_GLattr attribute, s32 value)
    {
        s32 setValue = -1;

        if (SDL_GL_GetAttribute(attribute, &setValue) != 0)
        {
            LOG_ERROR("SDL_GL_GetAttribute Failed\n{}\n", SDL_GetError());
        }

        if (setValue != value)
        {
            LOG_ERROR("Set value does not match requested value! [Set={}] [Requested={}]\n", setValue, value);
        }
    }
}