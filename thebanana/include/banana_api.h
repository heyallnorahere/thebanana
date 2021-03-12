#pragma once
#ifdef BANANA_WINDOWS
#ifdef BANANA_BUILD
#define BANANA_API __declspec(dllexport)
#else
#define BANANA_API __declspec(dllimport)
#endif
#else
#ifdef BANANA_BUILD
#define BANANA_API __attribute__((__visibility__("default")))
#else
#define BANANA_API
#endif
#endif