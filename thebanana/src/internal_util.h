#pragma once
#include "debug_tools.h"
#include "graphics/mesh.h"
namespace thebanana {
	class model;
}
unsigned char* load_image_from_file(const std::string& path, int& width, int& height, int& channels, bool flip = false);
void free_image(void* pixels);
unsigned long long generate_uuid();
float get_current_time();
#define CURRENT_TIME(type) static_cast<type>(get_current_time())
using per_mesh_function = void(*)(aiMesh*, aiNode*, thebanana::model*,
	const std::vector<thebanana::graphics::mesh::vertex>&,
	const std::vector<unsigned int>&,
	const std::vector<thebanana::graphics::mesh::vertex_bone_data>&,
	void*);
void set_per_mesh_function(per_mesh_function pmf);
per_mesh_function get_per_mesh_function();
void set_user_data(void* data);
void* get_user_data();
template<typename T> inline void append_vector(std::vector<T>& dest, const std::vector<T>& src) {
	dest.insert(dest.end(), src.begin(), src.end());
}