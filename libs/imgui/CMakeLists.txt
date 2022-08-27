cmake_minimum_required(VERSION 3.0.0)
project(imgui VERSION 0.1.0)

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

# Source files
add_library(imgui SHARED
	imgui.cpp
	imgui_draw.cpp
	imgui_tables.cpp
	imgui_widgets.cpp
	backends/imgui_impl_sdl.cpp
	backends/imgui_impl_opengl3.cpp
	misc/imgui_stdlib.cpp
)

target_precompile_headers(imgui PRIVATE
	imconfig.h
	imgui.h
	imstb_rectpack.h
	imstb_truetype.h
	backends/imgui_impl_sdl.h
	backends/imgui_impl_opengl3_loader.h
	backends/imgui_impl_opengl3.h
	misc/imgui_stdlib.h
)

# SDL2 Includes
target_include_directories(imgui PRIVATE ${SDL2_INCLUDE_DIRS})
# ImGUI Includes
target_include_directories(imgui PUBLIC ./ imgui/backends imgui/misc)
# Define GL_GLEXT_PROTOTYPES
target_compile_definitions(imgui PRIVATE GL_GLEXT_PROTOTYPES=1)
# Disable warnings
target_compile_options(imgui PRIVATE -w)
# -O3 optimisations
target_compile_options(imgui PRIVATE -O3)
# Debug output
target_compile_options(imgui PRIVATE -g)
# Link libraries
target_link_libraries(imgui OpenGL::GL SDL2::SDL2)