#pragma once
#ifdef BANANA_BUILD
#ifdef BANANA_MACOSX
#include <glad/glad.h>
#else
#include <GL/glew.h>
#ifdef BANANA_WINDOWS
#include <GL/wglew.h>
#endif
#ifdef BANANA_LINUX
#include <GL/glxew.h>
#include <xcb/xcb.h>
#endif
#endif
#ifndef BANANA_WINDOWS
#include <unistd.h>
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