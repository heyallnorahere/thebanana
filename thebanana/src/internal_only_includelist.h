#pragma once
#ifdef BANANA_BUILD
#include <GL/glew.h>
#ifdef BANANA_WINDOWS
#include <GL/wglew.h>
#else
#include <unistd.h>
#endif
#ifdef BANANA_LINUX
#include <GL/glxew.h>
#include <xcb/xcb.h>
#endif
#include <imgui.h>
#ifdef BANANA_WINDOWS
#include <backends/imgui_impl_win32.h>
#endif
#ifdef BANANA_LINUX
#include <backends/imgui_impl_x11.h>
#endif
#include <backends/imgui_impl_opengl3.h>
#include <misc/cpp/imgui_stdlib.h>
#include <yaml-cpp/yaml.h>
#include <mono/jit/jit.h>
#include <mono/metadata/assembly.h>
#include <mono/metadata/debug-helpers.h>
#include <mono/metadata/attrdefs.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#endif