#pragma once
#include "gameobject.h"
class player;
class camera : public gameobject {
public:
	camera(player* p);
	virtual void update() override;
	virtual void render() override;
private:
	player* m_player;
};