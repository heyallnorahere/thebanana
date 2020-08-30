#pragma once
#include "gameobject.h"
class player : public gameobject {
public:
	player();
	virtual void update() override;
	virtual void render() override;
	virtual ~player() override;
};