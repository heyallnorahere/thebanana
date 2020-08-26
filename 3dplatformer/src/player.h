#pragma once
#include "gameobject.h"
class player : public gameobject {
public:
	player();
	virtual void update() override;
	virtual void render() override;
	virtual ~player() override;
private:
	glm::vec3 m_color;
};