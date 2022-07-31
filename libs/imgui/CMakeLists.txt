cmake_minimum_required(VERSION 3.0.0)
project(imgui VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(SDL2 REQUIRED)
find_package(OpenGL REQUIRED)

if (${CMAKE_CXX_COMPILER_ID} STREQUAL "GNU" OR ${CMAKE_CXX_COMPILER_ID} STREQUAL "Clang")
	set(CMAKE_CXX_FLAGS "-w -O3")
	set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS} -g")
else()
	message(FATAL_ERROR "Only GCC/Clang are supported")
endif()

add_library(imgui SHARED
	imgui.cpp
	imgui_draw.cpp
	imgui_tables.cpp
	imgui_widgets.cpp
	backends/imgui_impl_sdl.cpp
	backends/imgui_impl_opengl3.cpp
)

target_include_directories(imgui PRIVATE ${SDL2_INCLUDE_DIRS})
target_include_directories(imgui PUBLIC ./ imgui/backends)
target_compile_definitions(imgui PRIVATE GL_GLEXT_PROTOTYPES=1)

target_link_libraries(imgui OpenGL::GL SDL2::SDL2)