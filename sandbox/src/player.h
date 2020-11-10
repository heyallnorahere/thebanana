#pragma once
namespace thebanana {
	class gameobject;
}
class player_behavior : public thebanana::script {
public:
	player_behavior(thebanana::gameobject* object, thebanana::native_script_component* nsc);
	virtual void initialize() override;
	virtual void update() override;
private:
	void move(float yaw_offset, glm::vec3& translate, const float speed);
	bool m_walking;
	float m_last_walk_speed;
};