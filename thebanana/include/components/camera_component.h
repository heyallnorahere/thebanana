#pragma once
#include "component.h"
#include "banana_api.h"
namespace thebanana {
	// stock camera component
	class camera_component : public component {
	public:
		BANANA_API camera_component(gameobject* object);
		BANANA_API virtual void update() override;
		BANANA_API virtual void render() override;
		BANANA_API glm::mat4 calculate_projection();
		BANANA_API glm::mat4 calculate_view();
	private:
		glm::vec3 direction;
	};
}