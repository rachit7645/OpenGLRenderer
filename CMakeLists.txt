# TODO: Add install functionality

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
find_package(fmt REQUIRED)

# Add sub-projects
add_subdirectory(libs/)

# Source files
add_executable(engine
    Main.cpp
    Window.cpp
    Model.cpp
    Shader.cpp
    Texture.cpp
    VertexArray.cpp
    VertexBuffer.cpp
    Entity.cpp
    Camera.cpp
    RenderManager.cpp
    Mesh.cpp
    Player.cpp
    Files.cpp
    SkyboxShader.cpp
    SkyboxRenderer.cpp
    Skybox.cpp
    Inputs.cpp
    MatrixBuffer.cpp
    UniformBuffer.cpp
    LightsBuffer.cpp
    PointLight.cpp
    Resources.cpp
    Time.cpp
    GL.cpp
    FrameBuffer.cpp
    RenderBuffer.cpp
    WaterShader.cpp
    WaterRenderer.cpp
    WaterFrameBuffers.cpp
    SharedBuffer.cpp
    MeshTextures.cpp
    WaterTile.cpp
    ShaderStorageBuffer.cpp
    InstancedRenderer.cpp
    InstanceBuffer.cpp
    FastInstancedShader.cpp
    ShadowMap.cpp
    ShadowShader.cpp
    ShadowBuffer.cpp
    GBuffer.cpp
    GBufferShader.cpp
    LightingRenderer.cpp
    LightingShader.cpp
    Vertex.cpp
    IBLMaps.cpp
    ConverterShader.cpp
    ConvolutionShader.cpp
    PreFilterShader.cpp
    BRDFShader.cpp
    DirectionalLight.cpp
    SpotLight.cpp
    Settings.cpp
    Instance.cpp
    IBLRenderer.cpp
    GLM.cpp
    LightingBuffer.cpp
    PostProcessShader.cpp
    PostProcessRenderer.cpp
    BloomBuffer.cpp
    DownSampleShader.cpp
    UpSampleShader.cpp
    BloomRenderer.cpp
    AABB.cpp
    Plane.cpp
    Frustum.cpp
    Transform.cpp
    OmniShadowMap.cpp
    OmniShadowShader.cpp
    OmniShadowBuffer.cpp
    SpotShadowMap.cpp
    SpotShadowShader.cpp
    SpotShadowBuffer.cpp
    SDLUtil.cpp
)

# Precompiled Headers
target_precompile_headers(engine PRIVATE
    # ImGui Headers
    libs/imgui/imgui.h
    libs/imgui/imgui_internal.h
    libs/imgui/backends/imgui_impl_sdl2.h
    libs/imgui/backends/imgui_impl_opengl3.h
    libs/imgui/misc/cpp/imgui_stdlib.h
    # STB Headers
    libs/stb/stb_image.h
    # GLEW Headers
    <GL/glew.h>
    # Assimp Headers
    <assimp/Importer.hpp>
    <assimp/scene.h>
    <assimp/postprocess.h>
    <assimp/types.h>
    # FMT Headers
    <fmt/core.h>
    <fmt/format.h>
    <fmt/chrono.h>
    # GLM Headers
    <glm/vec2.hpp>
    <glm/vec3.hpp>
    <glm/vec4.hpp>
    <glm/mat4x4.hpp>
    <glm/ext/vector_int2.hpp>
    <glm/ext/vector_int4.hpp>
    <glm/ext/vector_uint3.hpp>
    <glm/trigonometric.hpp>
    <glm/geometric.hpp>
    <glm/gtc/matrix_transform.hpp>
    # SIMD JSON Headers
    libs/simdjson/singleheader/simdjson.h
    # C++ STL Headers
    <unordered_map>
    <vector>
    <random>
    <filesystem>
    <array>
    <memory>
    <string_view>
    <string>
    <fstream>
    <chrono>
    <sstream>
    # C STL Headers
    <cstdint>
    <cstddef>
    <cassert>
    <cmath>
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
# Enable SSE2
target_compile_options(engine PRIVATE -msse2)
# Link libraries
target_link_libraries(engine SDL2::SDL2 OpenGL::GL GLEW::GLEW fmt::fmt assimp imgui simdjson)