cmake_minimum_required(VERSION 3.0.0)
project(engine VERSION 0.1.0)

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(OpenGL_GL_PREFERENCE GLVND)

find_package(SDL2 REQUIRED)
find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
find_package(glm REQUIRED)
find_package(assimp REQUIRED)

add_subdirectory(libs/imgui/)

if (${CMAKE_CXX_COMPILER_ID} STREQUAL "GNU" OR ${CMAKE_CXX_COMPILER_ID} STREQUAL "Clang")
	set(CMAKE_CXX_FLAGS "-Wall -Wextra -Wpedantic -O3")
	set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS} -g -D_DEBUG")
else()
	message(FATAL_ERROR "Only GCC/Clang are supported")
endif()

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
)

target_include_directories(engine PUBLIC libs/)
target_link_libraries(engine OpenGL::GL GLEW::GLEW SDL2::SDL2 assimp imgui)