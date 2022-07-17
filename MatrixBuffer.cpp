#include "MatrixBuffer.h"

#include <GL/glew.h>

#include "Maths.h"
#include "Util.h"

using namespace Renderer;

using Entities::Camera;

MatrixBuffer::MatrixBuffer() : UniformBuffer(0, sizeof(glm::mat4) * 2 + sizeof(glm::vec4)) {}

void MatrixBuffer::LoadView(const Camera& camera)
{
	glBindBuffer(GL_UNIFORM_BUFFER, id);
	glm::mat4 view = Maths::CreateViewMatrix(camera);
	glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), reinterpret_cast<const void*>(&view[0][0]));
	glm::vec4 cameraPos = glm::vec4(camera.position, 1.0f);
	glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4) * 2, sizeof(glm::vec4), reinterpret_cast<const void*>(&cameraPos));
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void MatrixBuffer::LoadProjection(const glm::mat4& projection)
{
	glBindBuffer(GL_UNIFORM_BUFFER, id);
	glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), reinterpret_cast<const void*>(&projection[0][0]));
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}