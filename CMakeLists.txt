cmake_minimum_required(VERSION 3.0.0)
project(engine VERSION 0.1.0)

# MSVC is not supported
if (${CMAKE_CXX_COMPILER_ID} STREQUAL "MSVC")
	message(FATAL_ERROR "Only GCC/Clang are supported")
endif()

# Output executable/libraries to main build directory
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

# Require C++ 17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# For Linux, prefer modern opengl
set(OpenGL_GL_PREFERENCE GLVND)

# Required packages
find_package(SDL2 REQUIRED)
find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
find_package(glm REQUIRED)
find_package(assimp REQUIRED)

# Add sub-projects
add_subdirectory(libs/imgui/)

# Source files
add_executable(engine
	Main.cpp
	Window.cpp
	EntityRenderer.cpp
	Model.cpp
	Shader.cpp
	StaticShader.cpp
	Texture.cpp
	VertexArray.cpp
	VertexBuffer.cpp
	Entity.cpp
	Maths.cpp
	Camera.cpp
	MainLoop.cpp
	MasterRenderer.cpp
	Terrain.cpp
	TerrainShader.cpp
	TerrainRenderer.cpp
	Material.cpp 
	Mesh.cpp
	Player.cpp
	Files.cpp
	Image.cpp
	SkyboxShader.cpp
	SkyboxRenderer.cpp
	CubeMap.cpp
	Skybox.cpp
	Inputs.cpp
	MatrixBuffer.cpp
	UniformBuffer.cpp
	LightsBuffer.cpp
	Light.cpp
	Resources.cpp
	Time.cpp
	GL.cpp
	FrameBuffer.cpp
	GUI.cpp
	GUIShader.cpp
	GUIRenderer.cpp
	RenderBuffer.cpp
	WaterShader.cpp
	WaterRenderer.cpp
	WaterFrameBuffers.cpp
	SharedBuffer.cpp
	MeshTextures.cpp
	TerrainTextures.cpp
	WaterTile.cpp
)

# Precompiled Headers
target_precompile_headers(engine PRIVATE
	# Project Headers
	Camera.h
	CubeMap.h
	Entity.h
	EntityRenderer.h
	Files.h
	FrameBuffer.h
	GL.h
	GLM.h
	GUI.h
	GUIRenderer.h
	GUIShader.h
	Image.h
	Imgui.h
	Light.h
	LightsBuffer.h
	Log.h
	Material.h
	Maths.h
	MatrixBuffer.h
	Mesh.h
	MeshTextures.h
	Model.h
	Player.h
	RenderBuffer.h
	Resources.h
	Shader.h
	SharedBuffer.h
	Skybox.h
	SkyboxRenderer.h
	SkyboxShader.h
	StaticShader.h
	Terrain.h
	TerrainRenderer.h
	TerrainShader.h
	TerrainTextures.h
	Texture.h
	Time.h
	UniformBuffer.h
	Util.h
	VertexArray.h
	VertexBuffer.h
	WaterShader.h
	WaterTile.h
	# ImGui Headers
	libs/imgui/imgui.h
	libs/imgui/backends/imgui_impl_sdl.h
	libs/imgui/backends/imgui_impl_opengl3.h
	libs/imgui/misc/imgui_stdlib.h
	# STB Headers
	libs/stb/stb_image.h
	# GLEW Headers
	<GL/glew.h>
	# Assimp Headers
	<assimp/Importer.hpp>
	<assimp/scene.h>
	<assimp/postprocess.h>
	# STL Headers
	<unordered_map>
	<vector>
	<random>
	<filesystem>
	<cstdint>
	<cstddef>
	<cassert>
	<cmath>
	<array>
	<memory>
	<string_view>
	<string>
	<iostream>
	<fstream>
	<algorithm>
	<limits>
	<chrono>
	<ctime>
	<sstream>
)

# Library includes
target_include_directories(engine PRIVATE libs/)
# Define _DEBUG in debug mode
target_compile_definitions(engine PRIVATE _DEBUG)
# Enable all warnings
target_compile_options(engine PRIVATE -Wall)
# Enable extra warnings
target_compile_options(engine PRIVATE -Wextra)
# Enable pedantic warnings
target_compile_options(engine PRIVATE -Wpedantic)
# -O3 optimisations
target_compile_options(engine PRIVATE -O3)
# Enable debug output
target_compile_options(engine PRIVATE -g)
# Link libraries
target_link_libraries(engine OpenGL::GL GLEW::GLEW SDL2::SDL2 assimp imgui)