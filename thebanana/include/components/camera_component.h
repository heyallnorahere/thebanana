#pragma once
#include "component.h"
namespace thebanana {
	// stock camera component
	class camera_component : public component {
	public:
		camera_component(gameobject* object);
		virtual void update() override;
		virtual void render() override;
		glm::mat4 calculate_projection();
		glm::mat4 calculate_view();
	private:
		glm::vec3 direction;
	};
}