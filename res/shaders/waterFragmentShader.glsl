#version 430 core

const float WAVE_STRENGTH    = 0.02f;
const float REFLECT_STRENGTH = 0.7f;

in vec2 txCoords;
in vec3 toCameraVector;
in vec4 clipSpace;

uniform sampler2D reflectionTx;
uniform sampler2D refractionTx;
uniform sampler2D dudvMap;

uniform float moveFactor;

out vec4 outColor;

void main()
{
	vec2 NDC             = (clipSpace.xy / clipSpace.w) / 2.0f + 0.5f;
	vec2 reflectTxCoords = vec2(NDC.x, -NDC.y);
	vec2 refractTxCoords = vec2(NDC.x, NDC.y);

	vec2 distortion1 = texture(dudvMap, vec2(txCoords.x + moveFactor, txCoords.y)).rg;
	distortion1      = distortion1 * 2.0f - 1.0f;
	distortion1      = distortion1 * WAVE_STRENGTH;

	vec2 distortion2 = texture(dudvMap, vec2(-txCoords.x + moveFactor, txCoords.y + moveFactor)).rg;
	distortion2      = distortion2 * 2.0f - 1.0f;
	distortion2      = distortion2 * WAVE_STRENGTH;

	vec2 totalDistortion = distortion1 + distortion2;

	reflectTxCoords   += totalDistortion;
	reflectTxCoords.x = clamp(reflectTxCoords.x, 0.001f, 0.999f);
	reflectTxCoords.y = clamp(reflectTxCoords.y, -0.999f, -0.001f);

	refractTxCoords   += totalDistortion;
	refractTxCoords   = clamp(refractTxCoords, 0.001f, 0.999f);

	vec4 reflectColor = texture(reflectionTx, reflectTxCoords);
	vec4 refractColor = texture(refractionTx, refractTxCoords);

	float refractFactor = dot(toCameraVector, vec3(0.0f, 1.0f, 0.0f));
	refractFactor       = pow(refractFactor, REFLECT_STRENGTH);

	outColor = mix(reflectColor, refractColor, refractFactor);
	outColor = mix(outColor, vec4(0.0f, 0.3f, 0.5f, 1.0f), 0.2f);
}