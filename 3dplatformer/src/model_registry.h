#pragma once
#include "transform.h"
class model_registry {
public:
	struct model_descriptor {
		std::string name, path;
		gl_model_loader::process_texture_path_proc texture_proc;
		transform model_transform;
	};
	model_registry();
	model_registry(const std::vector<model_descriptor>& models);
	~model_registry();
	bool loaded();
	void reload(const std::vector<model_descriptor>& models);
	void draw(const std::string& name, double time, int m_animation_index, opengl_shader_library::shader* shader);
	transform get_transform(const std::string& name);
	const aiScene* get_scene(const std::string& name);
private:
	void load(const std::vector<model_descriptor>& models);
	void unload();
	std::map<std::string, std::unique_ptr<gl_model_loader::model>> models;
	std::vector<model_descriptor> descriptors;
	bool has_loaded;
};