#version 430 core

// Entity instance
struct Instance
{
    // Model matrix
    mat4 modelMatrix;
    // Transposed inverse model matrix
    mat4 normalMatrix;
};

// Instances data SSBO
layout(std430, binding = 3) readonly buffer InstanceData
{
    // Instance array
    Instance instances[];
};

// Spot shadow buffer
layout(std140, binding = 6) uniform SpotShadowBuffer
{
    mat4 spotShadowMatrix;
    int  lightIndex;
};

// Vertex inputs
layout (location = 0) in vec3 position;

// Entry point
void main()
{
    // Transform to shadow space
    gl_Position = spotShadowMatrix * instances[gl_InstanceID].modelMatrix * vec4(position, 1.0f);
}
