#include "pch.h"
#include "particlesystem/particlesystem_component.h"
#include "particlesystem/particle_component.h"
#include "util.h"
#include "basic_gameobject.h"
#include "debug_tools.h"
#include "internal_util.h"
namespace thebanana {
	namespace particlesystem {
		particlesystem_component::particlesystem_component(gameobject* obj) : component(obj) {
			this->add_property(new property<float>(0.2f, "Spawn interval"));
			this->add_property(new property<float>(1.f, "Particle lifespan"));
			this->add_property(new property<std::string>("", "Particle model name"));
			this->add_property(new property<glm::vec3>(glm::vec3(0.f), "Particle acceleration"));
			this->add_property(new property<float>(0.f, "Minimum particle size"));
			this->add_property(new property<float>(1.f, "Maximum particle size"));
			this->last_spawned = CURRENT_TIME(float);
		}
		void particlesystem_component::update() {
			float current_time = CURRENT_TIME(float);
			float interval = *this->get_property<float>("Spawn interval");
			float diff = fabs(current_time - this->last_spawned);
			if (diff > interval) {
				this->last_spawned = current_time;
				this->spawn_particle();
			}
			for (auto& p : this->particles) {
				p->update();
			}
			for (auto index : this->to_remove) {
				auto it = this->particles.begin();
				std::advance(it, index);
				auto& p = *it;
				this->particles.remove(p);
				for (auto& index : this->to_remove) {
					index--;
				}
			}
			this->to_remove.clear();
		}
		void particlesystem_component::render() {
			for (auto& p : this->particles) {
				p->render();
			}
		}
		size_t particlesystem_component::find_particle(gameobject* obj) {
			for (size_t i = 0; i < this->particles.size(); i++) {
				auto it = this->particles.begin();
				std::advance(it, i);
				auto& p = *it;
				if (p.get() == obj) return i;
			}
			return std::string::npos;
		}
		void particlesystem_component::remove_particle(size_t index) {
			assert(index != std::string::npos);
			this->to_remove.push_back(index);
		}
		void particlesystem_component::spawn_particle() {
 			gameobject* particle = new basic_gameobject;
			particle->init(this->parent, this->parent->get_scene(), this->parent->get_game());
			particle->add_component<particle_component>().set_parent(this);
			this->particles.push_back(std::unique_ptr<gameobject>(particle));
		}
	}
}