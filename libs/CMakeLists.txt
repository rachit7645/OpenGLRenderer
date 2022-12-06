cmake_minimum_required(VERSION 3.0.0)
project(libs VERSION 0.1.0)

# MSVC is not supported
if (${CMAKE_CXX_COMPILER_ID} STREQUAL "MSVC")
	message(FATAL_ERROR "Only GCC/Clang are supported")
endif()

# Require C++ 17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# For Linux, prefer modern opengl
set(OpenGL_GL_PREFERENCE GLVND)

# ImGUI implementation
include(imgui.cmake)
# SIMD JSON implementation
include(simdjson.cmake)