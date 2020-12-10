#pragma once
#include "gameobject.h"
#include "banana_api.h"
namespace thebanana {
	class basic_gameobject : public gameobject {
	public:
		BANANA_API basic_gameobject();
		BANANA_API virtual void update() override;
		BANANA_API virtual void render() override;
	};
}