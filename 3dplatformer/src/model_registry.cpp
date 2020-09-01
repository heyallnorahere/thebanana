#include "pch.h"
#include "model_registry.h"
model_registry::model_registry() {
	this->unload();
}
model_registry::model_registry(const std::vector<model_descriptor>& models) : model_registry() {
	this->load(models);
}
model_registry::~model_registry() {
	this->unload();
}
bool model_registry::loaded() {
	return this->has_loaded;
}
void model_registry::reload(const std::vector<model_descriptor>& models) {
	this->unload();
	this->load(models);
}
void model_registry::draw(const std::string& name, double time, int m_animation_index, opengl_shader_library::shader* shader) {
	const auto& m = this->models[name];
	if (!m.get()) return;
	m->draw(shader->get_id(), static_cast<float>(time), m_animation_index);
}
transform model_registry::get_transform(const std::string& name) {
	size_t index = std::string::npos;
	for (size_t i = 0; i < this->descriptors.size(); i++) {
		if (this->descriptors[i].name == name) {
			index = i;
			break;
		}
	}
	if (index == std::string::npos) {
		return transform();
	}
	return this->descriptors[index].model_transform;
}
const aiScene* model_registry::get_scene(const std::string& name) {
	const auto& m = this->models[name];
	if (!m.get()) return NULL;
	return m->get_scene();
}
void model_registry::load(const std::vector<model_descriptor>& models) {
	for (auto md : models) {
		gl_model_loader::set_texture_path_proc(md.texture_proc, &md);
		this->models[md.name] = std::unique_ptr<gl_model_loader::model>(new gl_model_loader::model(md.path));
	}
	gl_model_loader::clear_texture_path_proc();
	this->descriptors = models;
	this->has_loaded = true;
}
void model_registry::unload() {
	this->models.clear();
	this->has_loaded = false;
}
