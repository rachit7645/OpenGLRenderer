#include "BRDFShader.h"

using namespace Shader;

constexpr auto VERTEX_PATH = "shaders/ComputeBRDF_VS.glsl",
    FRAGMENT_PATH = "shaders/ComputeBRDF_FS.glsl";

BRDFShader::BRDFShader()
    : ShaderProgram(VERTEX_PATH, FRAGMENT_PATH)
{
    GetUniformLocations();
}

void BRDFShader::GetUniformLocations()
{
}