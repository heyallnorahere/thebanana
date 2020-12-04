#pragma once
#include "component.h"
namespace thebanana {
	// stock camera component
	class camera_component : public component {
	public:
		camera_component(gameobject* object);
		virtual void update() override;
		virtual void render() override;
	private:
		glm::vec3 direction;
	};
}