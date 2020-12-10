#pragma once
#ifdef BANANA_BUILD
#include <GL/glew.h>
#include <imgui.h>
#include <backends/imgui_impl_win32.h>
#include <backends/imgui_impl_opengl3.h>
#include <misc/cpp/imgui_stdlib.h>
#include <steam/steam_api.h>
#include <yaml-cpp/yaml.h>
#include <model_loader.h>
#include <opengl_viewport.h>
#include <opengl_shader.h>
#include <include/gpu/GrBackendSurface.h>
#include <include/gpu/GrDirectContext.h>
#include <include/core/SkCanvas.h>
#include <include/core/SkFont.h>
#include <include/core/SkSurface.h>
#include <include/utils/SkRandom.h>
#include <include/gpu/gl/GrGLInterface.h>
#include <AL/al.h>
#include <AL/alc.h>
#ifdef __cplusplus
extern "C" {
#endif
#include <lua.h>
#include <lualib.h>
#ifdef __cplusplus
}
#endif
#include <lua.hpp>
#endif