#pragma once
#include "component.h"
namespace thebanana {
	namespace particlesystem {
		class particlesystem_component;
		class particle_component : public component {
		public:
			particle_component(gameobject* obj);
			virtual void update() override;
			particle_component& set_parent(particlesystem_component* psc);
		private:
			particlesystem_component* psc;
			float start_time;
			std::string last_frame_mesh_name;
		};
	}
}