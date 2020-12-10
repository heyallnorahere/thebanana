#pragma once
#ifdef BANANA_BUILD
#define BANANA_API __declspec(dllexport)
#else
#define BANANA_API __declspec(dllimport)
#endif