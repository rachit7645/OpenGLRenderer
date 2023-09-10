#ifndef DRAW_CALL_H
#define DRAW_CALL_H

#include "GL.h"

namespace Renderer
{
    // Constants
    constexpr usize MAX_DRAW_CALLS = 16384;

    // Draw call for MultiDrawIndirect
    struct __attribute__((packed)) DrawCall
    {
        GLuint count         = 0;
        GLuint instanceCount = 0;
        GLuint firstIndex    = 0;
        GLint  baseVertex    = 0;
        GLuint baseInstance  = 0;
    };
}

#endif
