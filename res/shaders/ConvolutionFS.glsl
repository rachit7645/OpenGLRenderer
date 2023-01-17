#version 430 core

// Constanats
const float PI           = 3.14159265359;
const float SAMPLE_DELTA = 0.025f;

// Vertex inputs
in vec3 worldPos;

// Uniforms
uniform samplerCube envMap;

// Fragment outputs
layout (location = 0) out vec3 outColor;

// Entry point
void main()
{
	// Get normal
	vec3 normal = normalize(worldPos);

	// Calculate direction vectors
	vec3 up    = vec3(0.0f, 1.0f, 0.0f);
	vec3 right = normalize(cross(up, normal));
	up         = normalize(cross(normal, right));

	// Set integration variables
	vec3  irradiance  = vec3(0.0f);
	float nrSamples   = 0.0f;

	// Loop over all phi
	for(float phi = 0.0; phi < 2.0f * PI; phi += SAMPLE_DELTA)
	{
		// Pre-calculate ratios
		float sinPhi = sin(phi);
		float cosPhi = cos(phi);
		// Loop over all theta
		for(float theta = 0.0f; theta < 0.5f * PI; theta += SAMPLE_DELTA)
		{
			// Pre-calculate ratios
			float sinTheta = sin(theta);
			float cosTheta = cos(theta);
			// Calculate irradiance
			vec3 tangentSample = vec3(sinTheta * cosPhi, sinTheta * sinPhi, cosTheta);
			vec3 sampleVec     = tangentSample.x * right + tangentSample.y * up + tangentSample.z * normal;
			irradiance        += texture(envMap, sampleVec).rgb * cosTheta * sinTheta;
			nrSamples         += 1;
		}
	}

	// Finalise irradiance
	irradiance = PI * irradiance * (1.0f / float(nrSamples));
	// Output color
	outColor = irradiance;
}