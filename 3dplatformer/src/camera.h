#pragma once
#include "gameobject.h"
class player;
class camera : public gameobject {
public:
	camera(player* p);
	virtual void update() override;
	virtual void render() override;
	glm::vec3& get_camera_offset();
	const glm::vec3& get_camera_offset() const;
private:
	player* m_player;
	glm::vec3 offset;
};