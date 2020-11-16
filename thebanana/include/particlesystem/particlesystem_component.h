#pragma once
#include "component.h"
namespace thebanana {
	namespace particlesystem {
		class particlesystem_component : public component {
		public:
			particlesystem_component(gameobject* obj);
			virtual void update() override;
			virtual void render() override;
			size_t find_particle(gameobject* obj);
			void remove_particle(size_t index);
		private:
			void spawn_particle();
			std::list<std::unique_ptr<gameobject>> particles;
			std::vector<size_t> to_remove;
			float last_spawned;
		};
	}
}