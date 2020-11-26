#pragma once
#include "transform.h"
namespace thebanana {
	class model_registry {
	public:
		using texture_path_proc = std::string(*)(const std::string&, void*);
		struct model_descriptor {
			std::string name, path;
			texture_path_proc texture_proc;
			transform model_transform;
		};
#ifdef BANANA_BUILD
		struct model_vertex_data {
			std::vector<gl_model_loader::vertex> vertices;
			std::vector<unsigned int> indices;
			std::vector<gl_model_loader::vbd> bone_data;
		};
#else
		struct model_vertex_data {
			std::vector<int> unused[3];
		};
#endif
		model_registry();
		model_registry(const std::vector<model_descriptor>& models);
		~model_registry();
		bool loaded();
		void reload(const std::vector<model_descriptor>& models);
		void draw(const std::string& name, double time, int m_animation_index, opengl_shader_library::shader* shader);
		transform get_transform(const std::string& name);
#ifdef BANANA_BUILD
		const aiScene* get_scene(const std::string& name);
		void add_vertex_data(const std::string& model_name, model_vertex_data data);
		model_vertex_data get_vertex_data(const std::string& model_name);
#endif
		static std::string path_helper(const std::string& original, const std::string& find, const std::string& replace);
		void load(const std::vector<model_descriptor>& models);
		std::vector<std::string> get_loaded_model_names() const;
	private:
		void unload();
#ifdef BANANA_BUILD
		using model_ptr = std::unique_ptr<gl_model_loader::model>;
#else
		using model_ptr = void*;
#endif
		std::map<std::string, model_vertex_data> vertex_data;
		std::map<std::string, model_ptr> models;
		std::vector<model_descriptor> descriptors;
		bool has_loaded;
	};
}