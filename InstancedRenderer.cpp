#include "InstancedRenderer.h"

using namespace Renderer;

InstancedRenderer::InstancedRenderer(Shader::InstancedShader& shader)
	: shader(shader)
{
	shader.Start();
	shader.ConnectTextureUnits();
	shader.Stop();
}

void InstancedRenderer::Render(const Batch& batch)
{

}
