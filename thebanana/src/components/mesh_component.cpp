#include "pch.h"
#include "components/mesh_component.h"
#include "components/animation_component.h"
#include "gameobject.h"
#include "game.h"
#include "scene.h"
#include "material.h"
namespace thebanana {
	mesh_component::mesh_component(gameobject* obj) : component(obj) {
		this->model_name = "none";
		this->add_property(new property<std::string>(this->model_name, "Mesh name"));
		this->add_property(new property<material*>(NULL, "Material"));
	}
	mesh_component& mesh_component::set_mesh_name(const std::string& name) {
		this->model_name = name;
		this->set_property<std::string>("Mesh name", name);
		return *this;
	}
	std::string mesh_component::get_mesh_name() const {
		return this->model_name;
	}
	material* mesh_component::get_material() {
		return *this->get_property<material*>("Material");
	}
	void mesh_component::pre_render() {
		property<std::string>* mn = this->find_property<std::string>("Mesh name");
		if (mn) {
			if (this->model_name != *mn->get_value()) {
				this->model_name = *mn->get_value();
			}
		}
	}
	void mesh_component::render() {
		if (this->model_name != "none") {
			material* mat = *this->get_property<material*>("Material");
			if (mat) {
				mat->send_to_shader(this->parent->get_scene()->get_shader()->get_id(), "shader_material");
			}
			// model_transform is the transform passed in to the model registry, transformed by the root transform of the aiScene
			transform model_transform = this->parent->get_game()->get_model_registry()->get_transform(this->model_name);
			// pass it in to the shader
			this->parent->get_scene()->get_shader()->get_uniforms().mat4("model_transform", model_transform.get_matrix());
			// get the animation index and the time relative to the start of the animation (if the parent object has an animation_component)
			double animation_time = this->get_number_components<animation_component>() > 0 ? this->get_component<animation_component>().get_animation_time() : 0.0;
			int animation_index = -1;
			if (this->get_number_components<animation_component>() > 0) {
				animation_index = this->get_component<animation_component>().get_animation_id();
			}
			// draw the model! (via my model loader library)
			this->parent->get_game()->get_model_registry()->draw(this->model_name, animation_time, animation_index, this->parent->get_scene()->get_shader());
		}
	}
}