#pragma once
#include "debug_tools.h"
unsigned char* load_image_from_file(const std::string& path, int& width, int& height, int& channels, bool flip = false);
void free_image(void* pixels);
unsigned long long generate_uuid();
float get_current_time();
__declspec(dllexport) bool is_2d(unsigned int texture);
#define CURRENT_TIME(type) static_cast<type>(get_current_time())