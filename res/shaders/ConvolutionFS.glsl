#version 430 core

const float PI = 3.14159265359;

in vec3 worldPos;

uniform samplerCube envMap;

out vec3 outColor;

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
	float sampleDelta = 0.025f;
	float nrSamples   = 0.0f;

	for(float phi = 0.0; phi < 2.0f * PI; phi += sampleDelta)
	{
		for(float theta = 0.0f; theta < 0.5f * PI; theta += sampleDelta)
		{
			// Pre-calculate ratios
			float sinPhi   = sin(phi);
			float cosPhi   = cos(phi);
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