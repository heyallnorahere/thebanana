#pragma once
#include "component.h"
#include "graphics/framebuffer.h"
#include "banana_api.h"
namespace thebanana {
	class light_component : public component {
	public:
		struct light_data {
			glm::vec3 position;
			glm::vec3 diffuse, specular, ambient;
			float ambient_strength;
			std::vector<glm::mat4> shadow_transforms;
			void* shadowmap;
		};
		BANANA_API light_component(gameobject* object);
		BANANA_API light_data get_data();
		BANANA_API graphics::framebuffer* get_depthbuffer();
		BANANA_API void set_shadow_transforms(const std::vector<glm::mat4>& transforms);
	private:
		std::unique_ptr<graphics::framebuffer> m_depthbuffer;
		std::vector<glm::mat4> m_shadow_transforms;
	};
}