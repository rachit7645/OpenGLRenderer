#version 430 core

// Texture tiling factor
const float TEXTURE_TILING = 4.0f;

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

// Shared buffer
layout(std140, binding = 2) uniform Shared
{
    vec4 clipPlane;
    vec4 cameraPos;
    vec4 resolution;
};

// Vertex inputs
layout(location = 0) in vec2 position;

// Uniforms
uniform mat4 modelMatrix;

// Vertex outputs
out vec4 worldPos;
out vec4 clipSpace;
out vec3 unitNormal;
out vec3 unitCameraVector;
out vec2 txCoords;

// Data functions
void CalculateTxCoords();
void CalculateLighting();

// Entry point
void main()
{
    // Calculate position
    worldPos    = modelMatrix * vec4(position.x, 0.0f, position.y, 1.0f);
    clipSpace   = projection * cameraView * worldPos;
    gl_Position = clipSpace;
    // Calculate texture coords
    CalculateTxCoords();
    // Calculate lighting data
    CalculateLighting();
}

void CalculateTxCoords()
{
    // Generate texture coordinates
    txCoords = position / vec2(2.0f) + vec2(0.5f);
    txCoords = txCoords * TEXTURE_TILING;
}

void CalculateLighting()
{
    // Transform normal
    vec4 transNormal = modelMatrix * vec4(0.0f, 1.0f, 0.0f, 0.0f);
    // Normalise normal
    unitNormal = normalize(transNormal.xyz);
    // Calculate camera vector
    unitCameraVector = normalize(cameraPos.xyz - worldPos.xyz);
}