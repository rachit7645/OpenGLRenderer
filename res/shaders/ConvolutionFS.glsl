#version 430 core

const float PI = 3.14159265359;

in vec3 worldPos;

uniform samplerCube envMap;

out vec4 outColor;

void main()
{
	vec3 normal = normalize(worldPos);

	vec3 up    = vec3(0.0f, 1.0f, 0.0f);
	vec3 right = normalize(cross(up, normal));
	up         = normalize(cross(normal, right));

	vec3  irradiance  = vec3(0.0f);
	float sampleDelta = 0.025f;
	float nrSamples   = 0.0f;

	for(float phi = 0.0; phi < 2.0f * PI; phi += sampleDelta)
	{
		for(float theta = 0.0f; theta < 0.5f * PI; theta += sampleDelta)
		{
			vec3 tangentSample = vec3(sin(theta) * cos(phi), sin(theta) * sin(phi), cos(theta));
			vec3 sampleVec     = tangentSample.x * right + tangentSample.y * up + tangentSample.z * normal;
			irradiance        += texture(envMap, sampleVec).rgb * cos(theta) * sin(theta);
			nrSamples         += 1;
		}
	}

	irradiance = PI * irradiance * (1.0f / float(nrSamples));

	outColor = vec4(irradiance, 1.0f);
}