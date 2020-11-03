#include "pch.h"
#include "internal_util.h"
#include <stb_image.h>
unsigned char* load_image_from_file(const std::string& path, int& width, int& height, int& channels, bool flip) {
	stbi_set_flip_vertically_on_load(flip);
	return stbi_load(path.c_str(), &width, &height, &channels, NULL);
}
void free_image(void* pixels) {
	stbi_image_free(pixels);
}
