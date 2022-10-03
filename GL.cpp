#include "GL.h"

#include <unordered_map>
#include <array>

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

	LOG_GL("{} {} {} [ID={}]:\n{}\n", sourceStr, typeStr, severityStr, id, message);
}

GLint GL::GetIntegerv(GLenum param)
{
	GLint value;
	glGetIntegerv(param, &value);
	return value;
}

std::string_view GL::GetString(GLenum name)
{
	return std::string_view(reinterpret_cast<const char*>(glGetString(name)));
}

void GL::Init(const glm::ivec2& dimensions)
{
	// Setup viewport
	glViewport(0, 0, dimensions.x, dimensions.y);
	// Enable MSAA
	glEnable(GL_MULTISAMPLE);
	// Enable Depth test
	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);
	// Enable back-face culling
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	// Enable Debug Output
	glEnable(GL_DEBUG_OUTPUT);
	glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
	// Set debug message callback
	glDebugMessageCallback(GL::CheckErrors, nullptr);
	// Message ids to filter
	constexpr std::array<GLuint, 1> ids =
	{
		131185
	};
	// Filter messages
	glDebugMessageControl
	(
		GL_DEBUG_SOURCE_API, // source
		GL_DEBUG_TYPE_OTHER, // type
		GL_DONT_CARE,        // severity
		ids.size(),          // count
		ids.data(),          // ids
		GL_FALSE             // enabled
	);
}

void GL::LogDebugInfo()
{
	LOG_DEBUG("GL_VENDOR: {}\n"                     , GL::GetString(GL_VENDOR));
	LOG_DEBUG("GL_RENDERER: {}\n"                   , GL::GetString(GL_RENDERER));
	LOG_DEBUG("GL_VERSION: {}\n"                    , GL::GetString(GL_VERSION));
	LOG_DEBUG("GL_SHADING_LANGUAGE_VERSION: {}\n"   , GL::GetString(GL_SHADING_LANGUAGE_VERSION));
	LOG_DEBUG("GL_MAX_FRAMEBUFFER_HEIGHT: {}\n"     , GL::GetIntegerv(GL_MAX_FRAMEBUFFER_HEIGHT));
	LOG_DEBUG("GL_MAX_FRAMEBUFFER_HEIGHT: {}\n"     , GL::GetIntegerv(GL_MAX_FRAMEBUFFER_HEIGHT));
	LOG_DEBUG("GL_MAX_FRAMEBUFFER_WIDTH: {}\n"      , GL::GetIntegerv(GL_MAX_FRAMEBUFFER_WIDTH));
	LOG_DEBUG("GL_MAX_FRAMEBUFFER_SAMPLES: {}\n"    , GL::GetIntegerv(GL_MAX_FRAMEBUFFER_SAMPLES));
	LOG_DEBUG("GL_MAX_TEXTURE_SIZE: {}\n"           , GL::GetIntegerv(GL_MAX_TEXTURE_SIZE));
	LOG_DEBUG("GL_MAX_TEXTURE_BUFFER_SIZE: {}\n"    , GL::GetIntegerv(GL_MAX_TEXTURE_BUFFER_SIZE));
	LOG_DEBUG("GL_MAX_3D_TEXTURE_SIZE: {}\n"        , GL::GetIntegerv(GL_MAX_3D_TEXTURE_SIZE));
	LOG_DEBUG("GL_MAX_CUBE_MAP_TEXTURE_SIZE: {}\n"  , GL::GetIntegerv(GL_MAX_CUBE_MAP_TEXTURE_SIZE));
	LOG_DEBUG("GL_MAX_TEXTURE_LOD_BIAS: {}\n"       , GL::GetIntegerv(GL_MAX_TEXTURE_LOD_BIAS));
	LOG_DEBUG("GL_MAX_TEXTURE_MAX_ANISOTROPY: {}\n" , GL::GetIntegerv(GL_MAX_TEXTURE_MAX_ANISOTROPY));
	LOG_DEBUG("GL_MAX_SAMPLES: {}\n"                , GL::GetIntegerv(GL_MAX_SAMPLES));
	LOG_DEBUG("GL_MAX_UNIFORM_BLOCK_SIZE: {}\n"     , GL::GetIntegerv(GL_MAX_UNIFORM_BLOCK_SIZE));
	LOG_DEBUG("GL_MAX_UNIFORM_BUFFER_BINDINGS: {}\n", GL::GetIntegerv(GL_MAX_UNIFORM_BUFFER_BINDINGS));
	LOG_DEBUG("GL_MAX_UNIFORM_LOCATIONS: {}\n"      , GL::GetIntegerv(GL_MAX_UNIFORM_LOCATIONS));
	LOG_DEBUG("GL_MAX_VERTEX_UNIFORM_BLOCKS: {}\n"  , GL::GetIntegerv(GL_MAX_VERTEX_UNIFORM_BLOCKS));
	LOG_DEBUG("GL_MAX_FRAGMENT_UNIFORM_BLOCKS: {}\n", GL::GetIntegerv(GL_MAX_FRAGMENT_UNIFORM_BLOCKS));
	LOG_DEBUG("GL_MAX_VERTEX_ATTRIBS: {}\n"         , GL::GetIntegerv(GL_MAX_VERTEX_ATTRIBS));
}