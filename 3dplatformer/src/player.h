#pragma once
#include "gameobject.h"
namespace thebanana {
	class player : public gameobject {
	public:
		player();
		virtual void update() override;
		virtual void render() override;
		virtual ~player() override;
	private:
		void move(float yaw_offset, glm::vec3& translate, const float speed);
		bool m_walking;
		glm::vec2 m_last_angle;
		float m_last_walk_speed;
	};
}