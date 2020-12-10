#pragma once
#include "component.h"
#include "banana_api.h"
namespace thebanana {
	namespace particlesystem {
		class particlesystem_component : public component {
		public:
			BANANA_API particlesystem_component(gameobject* obj);
			BANANA_API virtual void update() override;
			BANANA_API virtual void render() override;
			BANANA_API size_t find_particle(gameobject* obj);
			BANANA_API void remove_particle(size_t index);
		private:
			void spawn_particle();
			std::list<std::unique_ptr<gameobject>> particles;
			std::vector<size_t> to_remove;
			float last_spawned;
		};
	}
}