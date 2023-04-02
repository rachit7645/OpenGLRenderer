#ifndef RACHIT_IMGUI_H
#define RACHIT_IMGUI_H

// TODO: Properly fix ImGui Internal header

// Fixes an error with imgui_internal.h
#define IMGUI_DEFINE_MATH_OPERATORS

#include "imgui/imgui_internal.h"
#include "imgui/imgui.h"
#include "imgui/backends/imgui_impl_sdl2.h"
#include "imgui/backends/imgui_impl_opengl3.h"
#include "imgui/misc/cpp/imgui_stdlib.h"

#endif