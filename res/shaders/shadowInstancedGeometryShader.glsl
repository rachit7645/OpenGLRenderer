#version 430 core

const int MAX_LAYER_COUNT = 16;

layout(triangles,      invocations = 5)  in;
layout(triangle_strip, max_vertices = 3) out;

layout (std140, binding = 4) uniform ShadowBuffer
{
	int   cascadeCount;
	mat4  shadowMatrices[MAX_LAYER_COUNT];
	float cascadeDistances[MAX_LAYER_COUNT];
};

void main()
{
	for (int i = 0; i < 3; ++i)
	{
		gl_Position = shadowMatrices[gl_InvocationID] * gl_in[i].gl_Position;
		gl_Layer    = gl_InvocationID;
		EmitVertex();
	}
	EndPrimitive();
}