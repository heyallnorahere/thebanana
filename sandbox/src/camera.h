#pragma once
#include "gameobject.h"
#include "components/native_script.h"
class camera_behavior : public thebanana::script {
public:
	camera_behavior(thebanana::gameobject* object);
	virtual void update() override;
	virtual void render() override;
	glm::vec3 get_direction();
	glm::vec2 get_angle();
	void set_player(thebanana::gameobject* p);
private:
	thebanana::gameobject* m_player;
	glm::vec3 m_direction;
	glm::vec2 m_angle;
};