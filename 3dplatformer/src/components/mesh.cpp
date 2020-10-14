#include "pch.h"
#include "mesh.h"
#include "animation.h"
#include "gameobject.h"
#include "game.h"
#include "scene.h"
namespace thebanana {
	mesh_component::mesh_component(gameobject* obj) : component(obj) {
		this->model_name = "none";
		this->add_property(new property<std::string>(this->model_name, "mesh name"));
	}
	mesh_component& mesh_component::set_mesh_name(const std::string& name) {
		this->model_name = name;
		this->set_property<std::string>("mesh name", name);
		return *this;
	}
	std::string mesh_component::get_mesh_name() const {
		return this->model_name;
	}
	void mesh_component::pre_render() {
		property<std::string>* mn = this->find_property<std::string>("mesh name");
		if (mn) {
			if (this->model_name != *mn->get_value()) {
				this->model_name = *mn->get_value();
			}
		}
	}
	void mesh_component::render() {
		if (this->model_name != "none") {
			transform model_transform = this->parent->get_game()->get_model_registry()->get_transform(this->model_name);
			const aiScene* scene = this->parent->get_game()->get_model_registry()->get_scene(this->model_name);
			if (scene) model_transform *= scene->mRootNode->mTransformation;
			this->parent->get_scene()->get_shader()->get_uniforms().mat4("model_transform", model_transform.get_matrix());
			double animation_time = this->get_number_components<animation_component>() > 0 ? this->get_component<animation_component>().get_animation_time() : 0.0;
			int animation_index = -1;
			if (this->get_number_components<animation_component>() > 0) {
				animation_index = this->get_component<animation_component>().get_animation_id();
			}
			this->parent->get_game()->get_model_registry()->draw(this->model_name, animation_time, animation_index, this->parent->get_scene()->get_shader());
		}
	}
}