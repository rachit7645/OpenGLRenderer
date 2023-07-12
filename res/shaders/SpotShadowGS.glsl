#version 430 core

// Constants
const int MAX_LIGHTS = 4;

// Spot light shadow struct
struct SpotShadow
{
    mat4 shadowMatrix;
    int  lightIndex;
};

// Spot shadow buffer
layout(std140, binding = 6) uniform SpotShadowBuffer
{
    SpotShadow spotShadowMaps[MAX_LIGHTS];
    int        currentIndex;
};

// Geometry inputs & outputs
layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

// Entry point
void main()
{
    // For each vertex
    for (int i = 0; i < 3; ++i)
    {
        // Output position
        gl_Position = spotShadowMaps[currentIndex].shadowMatrix * gl_in[i].gl_Position;
        // Set current layer
        gl_Layer = currentIndex;
        // Emit vertex
        EmitVertex();
    }

    // Finish primitive
    EndPrimitive();
}