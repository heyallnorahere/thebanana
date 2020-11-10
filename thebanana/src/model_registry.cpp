#include "pch.h"
#include "model_registry.h"
namespace thebanana {
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
		if (!this->has_loaded) return;
		const auto& m = this->models[name];
		if (!m.get()) return;
		m->draw(shader->get_id(), static_cast<float>(time), m_animation_index);
	}
	transform model_registry::get_transform(const std::string& name) {
		if (!this->has_loaded) return transform();
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
		transform model_transform = this->descriptors[index].model_transform;
		const aiScene* scene = this->get_scene(name);
		if (scene) model_transform *= scene->mRootNode->mTransformation;
		return model_transform;
	}
	const aiScene* model_registry::get_scene(const std::string& name) {
		if (!this->has_loaded) return NULL;
		const auto& m = this->models[name];
		if (!m.get()) return NULL;
		return m->get_scene();
	}
	void model_registry::add_vertex_data(const std::string& model_name, model_vertex_data data) {
		auto& vertices = this->vertex_data[model_name].vertices;
		vertices.insert(vertices.end(), data.vertices.begin(), data.vertices.end());
		auto& indices = this->vertex_data[model_name].indices;
		indices.insert(indices.end(), data.indices.begin(), data.indices.end());
		auto& bone_data = this->vertex_data[model_name].bone_data;
		bone_data.insert(bone_data.end(), data.bone_data.begin(), data.bone_data.end());
	}
	model_registry::model_vertex_data model_registry::get_vertex_data(const std::string& model_name) {
		if (!this->has_loaded) return model_vertex_data();
		return this->vertex_data[model_name];
	}
	std::string model_registry::path_helper(const std::string& original, const std::string& find, const std::string& replace) {
		size_t pos = std::string::npos;
		std::string result = original;
		do {
			if (pos != std::string::npos) {
				result = replace + original.substr(pos + find.length());
			}
			pos = original.find(find, pos + 1);
		} while (pos != std::string::npos);
		return result;
	}
	struct vertex_loader_params {
		model_registry* r;
		std::string n;
	};
	void vertex_loader(aiMesh*, aiNode*, gl_model_loader::model*, const std::vector<gl_model_loader::vertex>& vertices, const std::vector<unsigned int>& indices, const std::vector<gl_model_loader::texture>&, const std::vector<gl_model_loader::vbd>& bone_data, vertex_loader_params* params) {
		params->r->add_vertex_data(params->n, { vertices, indices, bone_data });
	}
	void model_registry::load(const std::vector<model_descriptor>& models) {
		for (auto m : models) {
			gl_model_loader::set_texture_path_proc(m.texture_proc, &m);
			gl_model_loader::clear_mesh_procs();
			vertex_loader_params* vlp = new vertex_loader_params{ this, m.name };
			gl_model_loader::add_mesh_proc((gl_model_loader::process_mesh_proc)vertex_loader, vlp);
			this->vertex_data[m.name] = model_vertex_data();
			this->models[m.name] = std::unique_ptr<gl_model_loader::model>(new gl_model_loader::model(m.path));
			delete vlp;
		}
		this->descriptors = models;
		this->has_loaded = true;
	}
	void model_registry::unload() {
		this->models.clear();
		this->has_loaded = false;
	}
}