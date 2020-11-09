#pragma once
unsigned char* load_image_from_file(const std::string& path, int& width, int& height, int& channels, bool flip = false);
void free_image(void* pixels);
unsigned long long generate_uuid();