cmake_minimum_required(VERSION 3.0.0)
project(libs VERSION 0.1.0)

# TODO: Separate libs into different files

# MSVC is not supported
if (${CMAKE_CXX_COMPILER_ID} STREQUAL "MSVC")
	message(FATAL_ERROR "Only GCC/Clang are supported")
endif()

# Require C++ 17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# For Linux, prefer modern opengl
set(OpenGL_GL_PREFERENCE GLVND)

# Required packages
find_package(SDL2 REQUIRED)
find_package(OpenGL REQUIRED)

# ImGui source files
add_library(imgui SHARED
	imgui/imgui.cpp
	imgui/imgui_draw.cpp
	imgui/imgui_tables.cpp
	imgui/imgui_widgets.cpp
	imgui/backends/imgui_impl_sdl.cpp
	imgui/backends/imgui_impl_opengl3.cpp
	imgui/misc/cpp/imgui_stdlib.cpp
)

# ImGui precompiled headers
target_precompile_headers(imgui PRIVATE
	imgui/imconfig.h
	imgui/imgui.h
	imgui/imstb_rectpack.h
	imgui/imstb_truetype.h
	imgui/backends/imgui_impl_sdl.h
	imgui/backends/imgui_impl_opengl3_loader.h
	imgui/backends/imgui_impl_opengl3.h
	imgui/misc/cpp/imgui_stdlib.h
)

# SDL2 Includes
target_include_directories(imgui PRIVATE ${SDL2_INCLUDE_DIRS})
# ImGUI Includes
target_include_directories(imgui PUBLIC imgui/ imgui/backends imgui/misc)
# Define GL_GLEXT_PROTOTYPES
target_compile_definitions(imgui PRIVATE GL_GLEXT_PROTOTYPES=1)
# Disable warnings
target_compile_options(imgui PRIVATE -w)
# -O3 optimisations
target_compile_options(imgui PRIVATE -O3)
# Debug output
target_compile_options(imgui PRIVATE -g)
# Enable SSE2
target_compile_options(imgui PRIVATE -msse2)
# Link libraries
target_link_libraries(imgui OpenGL::GL SDL2::SDL2)

# SIMD JSON source files
add_library(simdjson STATIC simdjson/singleheader/simdjson.cpp)

# SIMD JSON precompiled headers
target_precompile_headers(simdjson PRIVATE simdjson/singleheader/simdjson.h)

# SIMD JSON includes
target_include_directories(simdjson PUBLIC simdjson/singleheader/)
# Disable warnings
target_compile_options(simdjson PRIVATE -w)
# -O3 optimisations
target_compile_options(simdjson PRIVATE -O3)
# Debug output
target_compile_options(simdjson PRIVATE -g)
# Enable SSE2
target_compile_options(simdjson PRIVATE -msse2)
# SIMD JSON libs
target_link_libraries(simdjson)