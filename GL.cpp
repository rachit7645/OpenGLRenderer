#include "GL.h"

#include <unordered_map>

#include "Log.h"
#include "Util.h"

void GL::CheckErrors
(
	GLenum source,
	GLenum type,
	GLuint id,
	GLenum severity,
	UNUSED GLsizei length,
	const GLchar* message,
	UNUSED const void* userParam
)
{
	// Disable certain messages
	if (id == 131185)
	{
		return;
	}

	static const std::unordered_map<GLenum, const char*>
	GL_ERROR_SOURCES =
	{
		{GL_DEBUG_SOURCE_API,            "[SOURCE_API]"     },
		{GL_DEBUG_SOURCE_WINDOW_SYSTEM,  "[WINDOW_SYSTEM]"  },
		{GL_DEBUG_SOURCE_SHADER_COMPILER,"[SHADER_COMPILER]"},
		{GL_DEBUG_SOURCE_THIRD_PARTY,    "[THIRD_PARTY]"    },
		{GL_DEBUG_SOURCE_APPLICATION,    "[APPLICATION]"    },
		{GL_DEBUG_SOURCE_OTHER,          "[OTHER]"          }
	};

	static const std::unordered_map<GLenum, const char*>
	GL_ERROR_TYPES =
	{
		{GL_DEBUG_TYPE_ERROR,               "[ERROR]"              },
		{GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR, "[DEPRECATED_BEHAVIOR]"},
		{GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR,  "[UNDEFINED_BEHAVIOR]" },
		{GL_DEBUG_TYPE_PORTABILITY,         "[PORTABILITY]"        },
		{GL_DEBUG_TYPE_PERFORMANCE,         "[PERFORMANCE]"        },
		{GL_DEBUG_TYPE_MARKER,              "[MARKER]"             },
		{GL_DEBUG_TYPE_PUSH_GROUP,          "[PUSH_GROUP]"         },
		{GL_DEBUG_TYPE_POP_GROUP,           "[POP_GROUP]"          },
		{GL_DEBUG_TYPE_OTHER,               "[OTHER]"              }
	};

	static const std::unordered_map<GLenum, const char*>
	GL_ERROR_SEVERITY =
	{
		{GL_DEBUG_SEVERITY_HIGH,         "[SEVERITY_HIGH]"        },
		{GL_DEBUG_SEVERITY_MEDIUM,       "[SEVERITY_MEDIUM]"      },
		{GL_DEBUG_SEVERITY_LOW,          "[SEVERITY_LOW]"         },
		{GL_DEBUG_SEVERITY_NOTIFICATION, "[SEVERITY_NOTIFICATION]"}
	};

	auto sourceStr   = GL_ERROR_SOURCES.find(source)->second;
	auto typeStr     = GL_ERROR_TYPES.find(type)->second;
	auto severityStr = GL_ERROR_SEVERITY.find(severity)->second;

	LOG_GL(sourceStr, " ", typeStr, " ", severityStr, " [ID=", id, "]:\n", message, "\n");
}

GLint GL::GetIntegerv(GLenum param)
{
	GLint value;
	glGetIntegerv(param, &value);
	return value;
}

void GL::Init(const glm::ivec2& dimensions)
{
	// Setup viewport
	glViewport
	(
		0,
		0,
		dimensions.x,
		dimensions.y
	);
	// Enable MSAA
	glEnable(GL_MULTISAMPLE);
	// Enable Depth test
	glEnable(GL_DEPTH_TEST);
	// Enable back-face culling
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	// Enable Debug Output
	glEnable(GL_DEBUG_OUTPUT);
	glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
	glDebugMessageCallback(GL::CheckErrors, nullptr);
}

void GL::LogDebugInfo()
{
	LOG_DEBUG("GL_VENDOR: "                     , glGetString(GL_VENDOR),                          "\n");
	LOG_DEBUG("GL_RENDERER: "                   , glGetString(GL_RENDERER),                        "\n");
	LOG_DEBUG("GL_VERSION: "                    , glGetString(GL_VERSION),                         "\n");
	LOG_DEBUG("GL_SHADING_LANGUAGE_VERSION: "   , glGetString(GL_SHADING_LANGUAGE_VERSION),        "\n");
	LOG_DEBUG("GL_MAX_FRAMEBUFFER_HEIGHT: "     , GL::GetIntegerv(GL_MAX_FRAMEBUFFER_HEIGHT),      "\n");
	LOG_DEBUG("GL_MAX_FRAMEBUFFER_HEIGHT: "     , GL::GetIntegerv(GL_MAX_FRAMEBUFFER_HEIGHT),      "\n");
	LOG_DEBUG("GL_MAX_FRAMEBUFFER_WIDTH: "      , GL::GetIntegerv(GL_MAX_FRAMEBUFFER_WIDTH),       "\n");
	LOG_DEBUG("GL_MAX_FRAMEBUFFER_SAMPLES: "    , GL::GetIntegerv(GL_MAX_FRAMEBUFFER_SAMPLES),     "\n");
	LOG_DEBUG("GL_MAX_TEXTURE_SIZE: "           , GL::GetIntegerv(GL_MAX_TEXTURE_SIZE),            "\n");
	LOG_DEBUG("GL_MAX_TEXTURE_BUFFER_SIZE: "    , GL::GetIntegerv(GL_MAX_TEXTURE_BUFFER_SIZE),     "\n");
	LOG_DEBUG("GL_MAX_3D_TEXTURE_SIZE: "        , GL::GetIntegerv(GL_MAX_3D_TEXTURE_SIZE),         "\n");
	LOG_DEBUG("GL_MAX_CUBE_MAP_TEXTURE_SIZE: "  , GL::GetIntegerv(GL_MAX_CUBE_MAP_TEXTURE_SIZE),   "\n");
	LOG_DEBUG("GL_MAX_TEXTURE_LOD_BIAS: "       , GL::GetIntegerv(GL_MAX_TEXTURE_LOD_BIAS),        "\n");
	LOG_DEBUG("GL_MAX_TEXTURE_MAX_ANISOTROPY: " , GL::GetIntegerv(GL_MAX_TEXTURE_MAX_ANISOTROPY),  "\n");
	LOG_DEBUG("GL_MAX_SAMPLES: "                , GL::GetIntegerv(GL_MAX_SAMPLES),                 "\n");
	LOG_DEBUG("GL_MAX_UNIFORM_BLOCK_SIZE: "     , GL::GetIntegerv(GL_MAX_UNIFORM_BLOCK_SIZE),      "\n");
	LOG_DEBUG("GL_MAX_UNIFORM_BUFFER_BINDINGS: ", GL::GetIntegerv(GL_MAX_UNIFORM_BUFFER_BINDINGS), "\n");
	LOG_DEBUG("GL_MAX_UNIFORM_LOCATIONS: "      , GL::GetIntegerv(GL_MAX_UNIFORM_LOCATIONS),       "\n");
	LOG_DEBUG("GL_MAX_VERTEX_UNIFORM_BLOCKS: "  , GL::GetIntegerv(GL_MAX_VERTEX_UNIFORM_BLOCKS),   "\n");
	LOG_DEBUG("GL_MAX_FRAGMENT_UNIFORM_BLOCKS: ", GL::GetIntegerv(GL_MAX_FRAGMENT_UNIFORM_BLOCKS), "\n");
	LOG_DEBUG("GL_MAX_VERTEX_ATTRIBS: "         , GL::GetIntegerv(GL_MAX_VERTEX_ATTRIBS),          "\n");
}