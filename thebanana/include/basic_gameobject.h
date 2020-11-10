#pragma once
#include "gameobject.h"
namespace thebanana {
	class basic_gameobject : public gameobject {
	public:
		basic_gameobject();
		virtual void update() override;
		virtual void render() override;
	};
}