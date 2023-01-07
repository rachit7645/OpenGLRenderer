#version 430 core

const float TEXTURE_TILING = 4.0f;

layout(std140, binding = 0) uniform Matrices
{
	mat4 projectionMatrix;
	mat4 viewMatrix;
};

layout(std140, binding = 2) uniform Shared
{
	vec4 clipPlane;
	vec4 cameraPos;
	vec4 resolution;
};

layout(location = 0) in vec2 position;

uniform mat4 modelMatrix;

out vec4 worldPos;
out vec4 clipSpace;
out vec3 unitNormal;
out vec3 unitCameraVector;
out vec2 txCoords;

void CalculateTxCoords();
void CalculateLighting();

void main()
{
	worldPos    = modelMatrix * vec4(position.x, 0.0f, position.y, 1.0f);
	clipSpace   = projectionMatrix * viewMatrix * worldPos;
	gl_Position = clipSpace;

	CalculateTxCoords();
	CalculateLighting();
}

void CalculateTxCoords()
{
	txCoords = position / vec2(2.0f) + vec2(0.5f);
	txCoords = txCoords * TEXTURE_TILING;
}

void CalculateLighting()
{
	vec4 transNormal = modelMatrix * vec4(0.0f, 1.0f, 0.0f, 0.0f);
	unitNormal       = normalize(transNormal.xyz);
	unitCameraVector = normalize(cameraPos.xyz - worldPos.xyz);
}