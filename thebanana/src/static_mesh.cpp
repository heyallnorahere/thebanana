#include "pch.h"
#include "static_mesh.h"
#include "debug_tools.h"
#include "physics/rigidbody.h"
#include "components/mesh_component.h"
namespace thebanana {
	static_mesh::static_mesh(const std::string& model_name) {
		this->m_nickname = "Static mesh";
		this->add_tag("staticmesh");
		this->add_component<mesh_component>().set_mesh_name(model_name);
		this->add_component<rigidbody>().set_collision_model_name(model_name).get_property<component::property_base::dropdown>("Collision type")->set_item("Collide");
	}
	static_mesh::~static_mesh() { }
}