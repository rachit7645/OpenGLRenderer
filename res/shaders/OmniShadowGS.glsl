#version 430 core

// Primitive inputs
layout (triangles) in;

// Primitive outputs
layout (triangle_strip, max_vertices = 18) out;

layout (std140, binding = 5) uniform OmniShadowBuffer
{
    mat4 omniShadowMatrices[6];
    vec4 shadowPlanes;
    int  lightIndex;
};

// Geometry outputs
out vec3 fragPos;

// Entry point
void main()
{
    // For each face of the cube
    for(int face = 0; face < 6; ++face)
    {
        // Set layer (in this case, cube map face
        gl_Layer = face;

        // For each triangle
        for(int i = 0; i < 3; ++i) // for each triangle's vertices
        {
            // In-built input from vertex shader
            fragPos = gl_in[i].gl_Position.xyz;
            // Transform by shadow matrix
            gl_Position = omniShadowMatrices[face] * gl_in[i].gl_Position;
            // Output vertex
            EmitVertex();
        }

        // Output triangle
        EndPrimitive();
    }
}