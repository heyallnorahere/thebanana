#pragma once
#include "transform.h"
#include "model.h"
#include "banana_api.h"
namespace opengl_shader_library {
	class shader;
}
namespace thebanana {
	class model_registry {
	public:
		struct model_descriptor {
			std::string name, path;
			transform model_transform;
		};
		struct model_vertex_data {
			std::vector<graphics::mesh::vertex> vertices;
			std::vector<unsigned int> indices;
			std::vector<graphics::mesh::vertex_bone_data> bone_data;
		};
		BANANA_API model_registry();
		BANANA_API model_registry(const std::vector<model_descriptor>& models);
		BANANA_API ~model_registry();
		BANANA_API bool loaded();
		BANANA_API void reload(const std::vector<model_descriptor>& models);
		BANANA_API void draw(const std::string& name, double time, int m_animation_index);
		BANANA_API transform get_transform(const std::string& name);
#ifdef BANANA_BUILD
		const aiScene* get_scene(const std::string& name);
		void add_vertex_data(const std::string& model_name, model_vertex_data data);
		model_vertex_data get_vertex_data(const std::string& model_name);
#endif
		BANANA_API static std::string path_helper(const std::string& original, const std::string& find, const std::string& replace);
		BANANA_API void load(const std::vector<model_descriptor>& models);
		BANANA_API std::vector<std::string> get_loaded_model_names() const;
	private:
		void unload();
		std::map<std::string, model_vertex_data> vertex_data;
		std::map<std::string, std::unique_ptr<model>> models;
		std::vector<model_descriptor> descriptors;
		bool has_loaded;
	};
}