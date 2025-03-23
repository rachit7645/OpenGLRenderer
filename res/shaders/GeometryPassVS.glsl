#version 430 core

// Entity instance
struct Instance
{
    // Model matrix
    mat4 modelMatrix;
    // Transposed inverse model matrix
    mat4 normalMatrix;
};

// Matrix buffer
layout(std140, binding = 0) uniform Matrices
{
    // Regular matrices
    mat4 projection;
    mat4 cameraView;
    // Inverse matrices
    mat4 invProjection;
    mat4 invCameraView;
};

// Instance data SSBO
layout(std430, binding = 3) readonly buffer InstanceData
{
    // Instance array
    Instance instances[];
};

// Vertex inputs
layout(location = 0) in vec3 position;
layout(location = 1) in vec2 texCoords;
layout(location = 2) in vec3 normal;
layout(location = 3) in vec3 tangent;

// Vertex outputs
out vec2 txCoords;
out mat3 TBNMatrix;

// Entry point
void main()
{
    // Get instance
    Instance instance = instances[gl_InstanceID];
    // Transform vertex by model matrix
    vec4 fragPos = instance.modelMatrix * vec4(position, 1.0f);
    // Transform from world to clip space
    gl_Position = projection * cameraView * fragPos;
    // Pass through texture coords
    txCoords = texCoords;
    // Transform normal
    vec3 N = normalize(mat3(instance.normalMatrix) * normal);
    // Transform tangent
    vec3 T = normalize(instance.modelMatrix * vec4(tangent, 0.0f)).xyz;
    // Re-orthagonalize tangent
    T = normalize(T - dot(T, N) * N);
    // Calculate bitangent
    vec3 B = cross(N, T);
    // Compute TBN matrix
    TBNMatrix = mat3(T, B, N);
}