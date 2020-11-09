#pragma once
#include "components/native_script_component.h"
namespace thebanana {
	class gameobject;
}
class player_behavior : public thebanana::script {
public:
	player_behavior(thebanana::gameobject* object);
	virtual void update() override;
	void set_camera(thebanana::gameobject* c);
private:
	void move(float yaw_offset, glm::vec3& translate, const float speed);
	bool m_walking;
	glm::vec2 m_last_angle;
	float m_last_walk_speed;
	thebanana::gameobject* m_camera;
};