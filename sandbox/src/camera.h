#pragma once
class camera_behavior : public thebanana::script {
public:
	camera_behavior(thebanana::gameobject* object, thebanana::native_script_component* nsc);
	virtual void update() override;
	virtual void render() override;
	glm::vec3 get_direction();
	glm::vec2 get_angle();
};