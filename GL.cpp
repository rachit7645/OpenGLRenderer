#include "GL.h"

#include <unordered_map>

#include "Log.h"

void GL::CheckErrors
(
	GLenum source,
	GLenum type,
	UNUSED GLuint id,
	GLenum severity,
	UNUSED GLsizei length,
	const GLchar* message,
	UNUSED const void* userParam
)
{
	static const std::unordered_map<GLenum, const char*>
	GL_ERROR_SOURCES =
	{
		{GL_DEBUG_SOURCE_API,            "[GL_DEBUG_SOURCE_API]"            },
		{GL_DEBUG_SOURCE_WINDOW_SYSTEM,  "[GL_DEBUG_SOURCE_WINDOW_SYSTEM]"  },
		{GL_DEBUG_SOURCE_SHADER_COMPILER,"[GL_DEBUG_SOURCE_SHADER_COMPILER]"},
		{GL_DEBUG_SOURCE_THIRD_PARTY,    "[GL_DEBUG_SOURCE_THIRD_PARTY]"    },
		{GL_DEBUG_SOURCE_APPLICATION,    "[GL_DEBUG_SOURCE_APPLICATION]"    },
		{GL_DEBUG_SOURCE_OTHER,          "[GL_DEBUG_SOURCE_OTHER]"          }
	};

	static const std::unordered_map<GLenum, const char*>
	GL_ERROR_TYPES =
	{
		{GL_DEBUG_TYPE_ERROR,               "[GL_DEBUG_TYPE_ERROR]"              },
		{GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR, "[GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR]"},
		{GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR,  "[GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR]" },
		{GL_DEBUG_TYPE_PORTABILITY,         "[GL_DEBUG_TYPE_PORTABILITY]"        },
		{GL_DEBUG_TYPE_PERFORMANCE,         "[GL_DEBUG_TYPE_PERFORMANCE]"        },
		{GL_DEBUG_TYPE_MARKER,              "[GL_DEBUG_TYPE_MARKER]"             },
		{GL_DEBUG_TYPE_PUSH_GROUP,          "[GL_DEBUG_TYPE_PUSH_GROUP]"         },
		{GL_DEBUG_TYPE_POP_GROUP,           "[GL_DEBUG_TYPE_POP_GROUP]"          },
		{GL_DEBUG_TYPE_OTHER,               "[GL_DEBUG_TYPE_OTHER]"              }
	};

	static const std::unordered_map<GLenum, const char*>
	GL_ERROR_SEVERITY =
	{
		{GL_DEBUG_SEVERITY_HIGH,         "[GL_DEBUG_SEVERITY_HIGH]"        },
		{GL_DEBUG_SEVERITY_MEDIUM,       "[GL_DEBUG_SEVERITY_MEDIUM]"      },
		{GL_DEBUG_SEVERITY_LOW,          "[GL_DEBUG_SEVERITY_LOW]"         },
		{GL_DEBUG_SEVERITY_NOTIFICATION, "[GL_DEBUG_SEVERITY_NOTIFICATION]"}
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

void GL::DebugLog()
{
	LOG_DEBUG("GL_VENDOR: "                     , glGetString(GL_VENDOR),                           "\n");
	LOG_DEBUG("GL_RENDERER: "                   , glGetString(GL_RENDERER),                         "\n");
	LOG_DEBUG("GL_VERSION: "                    , glGetString(GL_VERSION),                          "\n");
	LOG_DEBUG("GL_SHADING_LANGUAGE_VERSION: "   , glGetString(GL_SHADING_LANGUAGE_VERSION),         "\n");
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