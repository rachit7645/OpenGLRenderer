#version 430 core

const float TEXTURE_TILING = 4.0f;
const int   MAX_LIGHTS     = 4;

struct Light
{
	vec4 position;
	vec4 color;
	vec4 attenuation;
};

layout(std140, binding = 0) uniform Matrices
{
	mat4 projectionMatrix;
	mat4 viewMatrix;
};

layout(std140, binding = 1) uniform Lights
{
	int   numLights;
	Light lights[MAX_LIGHTS];
};

layout(std140, binding = 2) uniform Shared
{
	vec4 clipPlane;
	vec4 skyColor;
	vec4 cameraPos;
	vec4 resolution;
};

layout(location = 0) in vec2 position;

uniform mat4 modelMatrix;

out vec4 clipSpace;
out vec3 unitCameraVector;
out vec3 unitLightVector;
out vec2 txCoords;

void CalculateTxCoords();
void CalculateLighting(vec4 worldPos);

void main()
{
	vec4 worldPos = modelMatrix * vec4(position.x, 0.0f, position.y, 1.0f);
	clipSpace     = projectionMatrix * viewMatrix * worldPos;
	gl_Position   = clipSpace;

	CalculateTxCoords();
	CalculateLighting(worldPos);
}

void CalculateTxCoords()
{
	txCoords = vec2(position.x / 2.0f + 0.5f, position.y / 2.0f + 0.5f);
	txCoords = txCoords * TEXTURE_TILING;
}

void CalculateLighting(vec4 worldPos)
{
	unitCameraVector = normalize(cameraPos.xyz - worldPos.xyz);
	unitLightVector  = normalize(lights[0].position.xyz - worldPos.xyz);
}