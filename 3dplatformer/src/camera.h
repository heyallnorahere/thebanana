#pragma once
#include "gameobject.h"
class camera : public thebanana::gameobject {
public:
	camera(thebanana::gameobject* p);
	virtual void update() override;
	virtual void render() override;
	glm::vec3 get_direction();
	glm::vec2 get_angle();
private:
	thebanana::gameobject* m_player;
	glm::vec3 m_direction;
	glm::vec2 m_angle;
};