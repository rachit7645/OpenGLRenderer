#version 430 core

// Struct describing an instance
struct Instance
{
	mat4 modelMatrix;
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
	gl_Position   = projection * cameraView * worldPosition;
	// Calculate clip plane
	gl_ClipDistance[0] = dot(worldPosition, clipPlane);
	// Interpolate texture coords
	txCoords = textureCoords;
	// Calculate normal
	vec3 transNormal = transpose(inverse(mat3(instances[gl_InstanceID].modelMatrix))) * normal;
	unitNormal       = normalize(transNormal);
	// Calculate view direction
	viewDir = normalize(cameraPos.xyz - worldPosition.xyz);
}