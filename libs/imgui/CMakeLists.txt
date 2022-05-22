cmake_minimum_required(VERSION 3.0.0)
project(imgui VERSION 0.1.0)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -w -std=c++17 -O3")

find_package(SDL2 REQUIRED)
find_package(OpenGL REQUIRED)

add_library(imgui STATIC imgui.cpp imgui_draw.cpp imgui_tables.cpp imgui_widgets.cpp backends/imgui_impl_sdl.cpp
	backends/imgui_impl_opengl3.cpp)

target_include_directories(imgui PUBLIC ${SDL2_INCLUDE_DIRS})	
target_include_directories(imgui PUBLIC ./ imgui/backends)
target_compile_definitions(imgui PUBLIC GL_GLEXT_PROTOTYPES=1)

target_link_libraries(imgui OpenGL::GL SDL2::SDL2)