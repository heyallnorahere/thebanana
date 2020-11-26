#include "pch.h"
#include "particlesystem/particle_component.h"
#include "particlesystem/particlesystem_component.h"
#include "util.h"
#include "gameobject.h"
#include "physics/rigidbody.h"
#include "components/components.h"
#include "internal_util.h"
namespace thebanana {
	namespace particlesystem {
		particle_component::particle_component(gameobject* obj) : component(obj) {
			this->start_time = CURRENT_TIME(float);
			this->parent->add_component<rigidbody>();
			this->parent->add_component<mesh_component>();
		}
		void particle_component::initialize() {
			this->parent->get_game()->get_rigidbody_list().remove(&this->get_component<rigidbody>());
		}
		void particle_component::update() {
			std::string mesh_name = *this->psc->get_property<std::string>("Particle model name");
			if (mesh_name != this->last_frame_mesh_name) {
				this->parent->get_component<mesh_component>().set_property<std::string>("Mesh name", mesh_name);
				this->last_frame_mesh_name = mesh_name;
			}
			float lifespan = *this->psc->get_property<float>("Particle lifespan");
			float current_time = CURRENT_TIME(float);
			if (current_time - this->start_time > lifespan) {
				this->psc->remove_particle(this->psc->find_particle(this->parent));
			}
			glm::vec3 force = *this->psc->get_property<glm::vec3>("Particle acceleration");
			this->parent->get_component<rigidbody>().apply_force(force);
		}
		particle_component& particle_component::set_parent(particlesystem_component* psc) {
			this->psc = psc;
			float percentage = static_cast<float>(rand() % 11) / 10.f;
			float min = *this->psc->get_property<float>("Minimum particle size");
			float max = *this->psc->get_property<float>("Maximum particle size");
			float scale = min + (percentage * (max - min));
			this->parent->get_transform().scale(scale);
			return *this;
		}
	}
}