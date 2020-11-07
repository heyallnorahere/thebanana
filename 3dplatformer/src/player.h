#pragma once
#include "components/native_script.h"
namespace thebanana {
	class gameobject;
}
class camera;
class player_behavior : public thebanana::script {
public:
	player_behavior(thebanana::gameobject* object);
	virtual void update() override;
	void set_camera(camera* c);
private:
	void move(float yaw_offset, glm::vec3& translate, const float speed);
	bool m_walking;
	glm::vec2 m_last_angle;
	float m_last_walk_speed;
	camera* m_camera;
};