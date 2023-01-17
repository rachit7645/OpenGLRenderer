#version 430 core

// Struct describing an instance
struct Instance
{
	mat4 modelMatrix;
};

// Matrix buffer
layout(std140, binding = 0) uniform Matrices
{
	mat4 projectionMatrix;
	mat4 viewMatrix;
};

// Shared buffer
layout(std140, binding = 2) uniform Shared
{
	vec4 clipPlane;
	vec4 cameraPos;
	vec4 resolution;
};

// Tnstance data SSBO
layout(std430, binding = 3) readonly buffer InstanceData
{
	Instance instances[];
};

// Vertex inputs
layout(location = 0) in vec3 position;
layout(location = 1) in vec2 textureCoords;
layout(location = 2) in vec3 normal;

// Vertex outputs
out vec2 txCoords;
out vec3 unitNormal;
out vec3 viewDir;
out vec4 worldPosition;

// Entry point
void main()
{
	// Calculate position
	worldPosition = instances[gl_InstanceID].modelMatrix * vec4(position, 1.0f);
	gl_Position   = projectionMatrix * viewMatrix * worldPosition;
	// Calculate clip plane
	gl_ClipDistance[0] = dot(worldPosition, clipPlane);
	// Interpolate texture coords
	txCoords = textureCoords;
	// Calculate normal
	vec4 transNormal = instances[gl_InstanceID].modelMatrix * vec4(normal, 0.0f);
	unitNormal       = normalize(transNormal.xyz);
	// Calculate view direction
	viewDir = normalize(cameraPos.xyz - worldPosition.xyz);
}