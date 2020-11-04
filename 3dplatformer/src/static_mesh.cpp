#include "pch.h"
#include "static_mesh.h"
#include "debug_tools.h"
#include "physics/rigidbody.h"
#include "components/mesh.h"
namespace thebanana {
	static_mesh::static_mesh(const std::string& model_name) {
		this->m_nickname = "static mesh";
		this->add_component<mesh_component>().set_mesh_name(model_name);
	}
	void static_mesh::update() {
		this->prepare_for_update();
		if (this->get_number_components<mesh_component>() > 0) {
			component::property<std::string>* mn = this->find_property<std::string>("model name");
			if (mn) {
				if (this->get_component<mesh_component>().get_mesh_name() != *mn->get_value()) {
					this->get_component<mesh_component>().set_mesh_name(*mn->get_value());
				}
			}
		}
		this->post_update();
	}
	void static_mesh::render() {
		this->prepare_for_render();
		component::property<std::string>* model_name = this->find_property<std::string>("model name");
		if (this->get_number_components<mesh_component>() > 0) {
			this->get_component<mesh_component>().render();
		}
		this->post_render();
	}
	static_mesh::~static_mesh() { }
}