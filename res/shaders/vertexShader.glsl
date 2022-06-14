#version 420 core

const float FOG_DENSITY  = 0.0035f;
const float FOG_GRADIENT = 1.5f;

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 textureCoords;
layout(location = 2) in vec3 normal;

layout(std140, binding = 0) uniform Matrices
{
	uniform mat4 projectionMatrix;
	uniform mat4 viewMatrix;
};

layout(std140, binding = 1) uniform Lights
{
	uniform vec4 lightPosition;
	uniform vec4 lightColour;
};

uniform mat4 modelMatrix;
uniform int useFakeLighting;

out vec2 pass_textureCoords;
out vec3 surfaceNormal;
out vec3 toLightVector;
out vec3 toCameraVector;
out float visibility;

void CalculateLighting(vec4 worldPosition);
void CalculateVisibility(vec4 positionRelativeToCamera);

void main()
{
	vec4 worldPosition = modelMatrix * vec4(position, 1.0f);
	vec4 positionRelativeToCamera = viewMatrix * worldPosition;
	gl_Position = projectionMatrix * positionRelativeToCamera;
	pass_textureCoords = textureCoords;

	CalculateLighting(worldPosition);
	CalculateVisibility(positionRelativeToCamera);
}

void CalculateLighting(vec4 worldPosition)
{
	// Remove one if statement with the mix function
	// (which hopefully is a single instruction or similar on the GPU)
	// Original:
	// vec3 actualNormal = normal;
	// if (useFakeLighting == 1) actualNormal = vec3(0.0f, 1.0f, 0.0f);
	vec3 actualNormal = mix(normal, vec3(0.0f, 1.0f, 0.0f), useFakeLighting);
	surfaceNormal = (modelMatrix * vec4(actualNormal, 0.0f)).xyz;
	toLightVector = lightPosition.xyz - worldPosition.xyz;
	// FIXME: Very expensive calculation ver vertex
	// Maybe upload inverse viewMatrix from cpu instead?
	toCameraVector = (inverse(viewMatrix) * vec4(0.0f, 0.0f, 0.0f, 1.0f)).xyz - worldPosition.xyz;
}

void CalculateVisibility(vec4 positionRelativeToCamera)
{
	float distance = length(positionRelativeToCamera.xyz);
	visibility = exp(-pow((distance * FOG_DENSITY), FOG_GRADIENT));
	visibility = clamp(visibility, 0.0f, 1.0f);
}