#pragma once
#include "component.h"
#include "banana_api.h"
namespace thebanana {
	namespace particlesystem {
		class particlesystem_component;
		class particle_component : public component {
		public:
			BANANA_API particle_component(gameobject* obj);
			BANANA_API virtual void initialize() override;
			BANANA_API virtual void update() override;
			BANANA_API particle_component& set_parent(particlesystem_component* psc);
		private:
			particlesystem_component* psc;
			float start_time;
			std::string last_frame_mesh_name;
		};
	}
}