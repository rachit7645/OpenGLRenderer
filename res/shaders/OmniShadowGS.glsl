#version 430 core

// Constants
const int MAX_LIGHTS = 4;

// Point Light Shadow Struct
struct PointShadow
{
    mat4 matrices[6];
    vec4 shadowPlanes;
    int  lightIndex;
};

// Primitive inputs
layout (triangles) in;

// Primitive outputs
layout (triangle_strip, max_vertices = 18) out;

layout (std140, binding = 5) uniform OmniShadowBuffer
{
    PointShadow omniShadowMaps[MAX_LIGHTS];
    int         currentIndex;
};

// Geometry outputs
out vec3 fragPos;

// Entry point
void main()
{
    // For each face of the cube
    for(int face = 0; face < 6; ++face)
    {
        // Set layer (see https://registry.khronos.org/OpenGL-Refpages/gl4/html/gl_Layer.xhtml)
        gl_Layer = currentIndex * 6 + face;

        // For each triangle
        for(int i = 0; i < 3; ++i) // for each triangle's vertices
        {
            // In-built input from vertex shader
            fragPos = gl_in[i].gl_Position.xyz;
            // Transform by shadow matrix
            gl_Position = omniShadowMaps[currentIndex].matrices[face] * gl_in[i].gl_Position;
            // Output vertex
            EmitVertex();
        }

        // Output triangle
        EndPrimitive();
    }
}