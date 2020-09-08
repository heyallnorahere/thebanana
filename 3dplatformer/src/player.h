#pragma once
#include "gameobject.h"
class player : public gameobject {
public:
	player();
	virtual void update() override;
	virtual void render() override;
	virtual ~player() override;
	virtual std::string get_model_name() override;
	virtual float get_last_walk_speed() override;
private:
	void move(float yaw_offset, glm::vec3& translate, const float speed);
	bool m_walking;
	glm::vec2 m_last_angle;
	float m_last_walk_speed;
};