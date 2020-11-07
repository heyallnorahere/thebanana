#pragma once
#include "gameobject.h"
class player;
class camera : public thebanana::gameobject {
public:
	camera(player* p);
	virtual void update() override;
	virtual void render() override;
	glm::vec3 get_direction();
	glm::vec2 get_angle();
private:
	player* m_player;
	glm::vec3 m_direction;
	glm::vec2 m_angle;
};