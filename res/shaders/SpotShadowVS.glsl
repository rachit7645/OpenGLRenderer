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

// Vertex inputs
layout (location = 0) in vec3 position;

// Entry point
void main()
{
    // Transform to shadow space
    gl_Position = instances[gl_InstanceID].modelMatrix * vec4(position, 1.0f);
}
