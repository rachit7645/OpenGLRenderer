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