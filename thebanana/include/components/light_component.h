#pragma once
#include "component.h"
#include "graphics/framebuffer.h"
#include "banana_api.h"
namespace thebanana {
	class light_component : public component {
	public:
		enum class light_type {
			none = 0,
			point = 0b01,
			directional = 0b10,
			spotlight = 0b11,
		};
		struct light_data {
			light_type type;
			glm::vec3 position, direction;
			glm::vec3 diffuse, specular, ambient;
			float ambient_strength;
			float cutoff;
			void* shadowmap;
			glm::mat4 light_space_matrix;
		};
		BANANA_API light_component(gameobject* object);
		BANANA_API virtual void update() override;
		BANANA_API light_data get_data();
		BANANA_API graphics::framebuffer* get_depthbuffer();
		BANANA_API void set_light_space_matrix(const glm::mat4& matrix);
		BANANA_API void update_depthbuffer();
	private:
		light_type get_type();
		light_type m_type;
		std::unique_ptr<graphics::framebuffer> m_depthbuffer;
		glm::mat4 m_light_space_matrix;
	};
}