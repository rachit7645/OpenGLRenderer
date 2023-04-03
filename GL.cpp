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
	// Error sources map
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

	// Error types map
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

	// Error severity map
	static const std::unordered_map<GLenum, const char*>
	GL_ERROR_SEVERITY =
	{
		{GL_DEBUG_SEVERITY_HIGH,         "[SEVERITY_HIGH]"        },
		{GL_DEBUG_SEVERITY_MEDIUM,       "[SEVERITY_MEDIUM]"      },
		{GL_DEBUG_SEVERITY_LOW,          "[SEVERITY_LOW]"         },
		{GL_DEBUG_SEVERITY_NOTIFICATION, "[SEVERITY_NOTIFICATION]"}
	};

	// Look up strings
	auto sourceStr   = GL_ERROR_SOURCES.find(source)->second;
	auto typeStr     = GL_ERROR_TYPES.find(type)->second;
	auto severityStr = GL_ERROR_SEVERITY.find(severity)->second;

	// Log
	LOG_GL("{} {} {} [ID={}]:\n{}\n", sourceStr, typeStr, severityStr, id, message);
}

GLint GL::GetIntegerv(GLenum param)
{
	// Store value
	GLint value;
	// Get value
	glGetIntegerv(param, &value);
	// Return value
	return value;
}

std::string_view GL::GetString(GLenum name)
{
	// Get string
	auto string = glGetString(name);
	// Convert to different format
	auto newString = reinterpret_cast<const char*>(string);
	// Return
	return std::string_view(newString);
}

void GL::Init(const glm::ivec2& dimensions)
{
	// Setup viewport
	glViewport(0, 0, dimensions.x, dimensions.y);

	// Enable Depth test
	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);
	glDepthRange(0.0, 1.0);

	// Enable back-face culling
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	// Enable seamless cube maps
	glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
	// Disable MSAA
	glDisable(GL_MULTISAMPLE);

	// Enable Debug Output
	glEnable(GL_DEBUG_OUTPUT);
	glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
	// Set debug message callback
	glDebugMessageCallback(GL::CheckErrors, nullptr);
	// Message ids to filter
	constexpr std::array<GLuint, 1> ids = {131185};
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

std::string_view GLEW::GetString(GLenum name)
{
	// Get string
	auto string = glewGetString(name);
	// Convert to different format
	auto newString = reinterpret_cast<const char*>(string);
	// Return
	return std::string_view(newString);
}

bool GLEW::GetExtension(const std::string_view name)
{
	// Return
	return glewGetExtension(name.data());
}