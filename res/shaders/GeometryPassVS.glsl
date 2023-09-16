#version 460 core

// Extensions
#extension GL_ARB_shader_draw_parameters : enable

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
out vec3 worldPos;
out mat3 TBNMatrix;

// Flat vertex outputs
out flat int drawID;

// Entry point
void main()
{
    // Get instance
    Instance instance = instances[gl_DrawID + gl_InstanceID];
    // Transform vertex by model matrix
    vec4 fragPos = instance.modelMatrix * vec4(position, 1.0f);
    // Set world position
    worldPos = fragPos.xyz;
    // Transform from world to clip space
    gl_Position = projection * cameraView * fragPos;
    // Pass through texture coords
    txCoords = texCoords;
    // Transform normal
    vec3 N = normalize(mat3(instance.normalMatrix) * normal);
    // Transform tangent
    vec3 T = normalize(mat3(instance.normalMatrix) * tangent);
    // Re-orthagonalize tangent
    T = normalize(T - dot(T, N) * N);
    // Calculate bitangent
    vec3 B = cross(N, T);
    // Compute TBN matrix
    TBNMatrix = mat3(T, B, N);
    // Set MDI draw ID
    drawID = gl_DrawID;
}