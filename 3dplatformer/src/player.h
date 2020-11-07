#pragma once
#include "gameobject.h"
class camera;
class player : public thebanana::gameobject {
public:
	player();
	virtual void update() override;
	virtual void render() override;
	virtual ~player() override;
	void set_camera(camera* c);
private:
	void move(float yaw_offset, glm::vec3& translate, const float speed);
	bool m_walking;
	glm::vec2 m_last_angle;
	float m_last_walk_speed;
	camera* m_camera;
};