#include "pch.h"
#include "internal_util.h"
#include "component.h"
#include "material.h"
#include <stb_image.h>
unsigned char* load_image_from_file(const std::string& path, int& width, int& height, int& channels, bool flip) {
	stbi_set_flip_vertically_on_load(flip);
	return stbi_load(path.c_str(), &width, &height, &channels, NULL);
}
void free_image(void* pixels) {
	stbi_image_free(pixels);
}
unsigned long long generate_uuid() {
	srand((unsigned int)get_current_time());
	unsigned long long uuid = 0;
	for (int i = 0; i < 10; i++) {
		int digit = rand() % 10;
		uuid += static_cast<unsigned long long>(digit) * pow(10, static_cast<unsigned long long>(i));
	}
	return uuid;
}
// so the linker doesnt freak out
thebanana::component::property<int> p1(0, "");
thebanana::component::property<bool> p2(false, "");
thebanana::component::property<float> p3(0.f, "");
thebanana::component::property<double> p4(0.0, "");
thebanana::component::property<glm::vec2> p5(glm::vec2(), "");
thebanana::component::property<glm::vec3> p6(glm::vec3(), "");
thebanana::component::property<glm::vec4> p7(glm::vec4(), "");
thebanana::component::property<thebanana::component::property_base::read_only_text> p8(thebanana::component::property_base::read_only_text(""), "");
thebanana::component::property<thebanana::component::property_base::dropdown> p9(thebanana::component::property_base::dropdown(std::vector<std::string>()), "");
thebanana::component::property<thebanana::component::property_base::color> p10(glm::vec3(0.f), "");
thebanana::component::property<thebanana::gameobject*> p11(NULL, "");
thebanana::component::property<thebanana::material*> p12(NULL, "");
float get_current_time() {
	static double start_time = 0;
	double raw_time = std::chrono::duration_cast<std::chrono::duration<double, std::ratio<1, 1>>>(std::chrono::system_clock::now().time_since_epoch()).count();
	if (start_time < 0.0001) {
		start_time = raw_time;
	}
	float time = static_cast<float>(raw_time - start_time);
	return time;
}
