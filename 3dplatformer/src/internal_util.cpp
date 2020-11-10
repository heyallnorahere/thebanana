#include "pch.h"
#include "internal_util.h"
#include "component.h"
#include <stb_image.h>
unsigned char* load_image_from_file(const std::string& path, int& width, int& height, int& channels, bool flip) {
	stbi_set_flip_vertically_on_load(flip);
	return stbi_load(path.c_str(), &width, &height, &channels, NULL);
}
void free_image(void* pixels) {
	stbi_image_free(pixels);
}
unsigned long long generate_uuid() {
	unsigned long long uuid = 0;
	for (int i = 0; i < 10; i++) {
		int digit = rand() % 10;
		uuid += static_cast<unsigned long long>(digit) * pow(10, static_cast<unsigned long long>(i));
	}
	return uuid;
}
// so the linker doesnt freak out
thebanana::component::property<double> p1(0.0, "");
thebanana::component::property<glm::vec2> p2(glm::vec2(), "");
thebanana::component::property<glm::vec3> p3(glm::vec3(), "");
