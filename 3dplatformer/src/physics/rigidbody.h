#pragma once
#include "component.h"
#include "model_registry.h"
#include "collider.h"
class rigidbody : public component {
public:
	rigidbody(gameobject* obj);
	virtual void initialize() override;
	virtual void pre_update() override;
	virtual void post_update() override;
	virtual void on_collision(gameobject* other) override;
	virtual void clean_up() override;
	template<typename _Ty> _Ty& set_collider_type();
	rigidbody& set_check_for_collisions(bool check);
	gameobject* get_parent();
	model_registry::model_vertex_data get_vertex_data();
	glm::vec3& get_shift_delta();
	int& get_num_collisions();
	void apply_force(const glm::vec3& force);
	rigidbody& set_collision_model_name(const std::string& model_name);
	std::string get_collision_model_name();
	static const std::vector<rigidbody*>& get_rigidbodies();
	static glm::vec3 gravity;
private:
	int num_collisions;
	glm::vec3 shift_delta;
	model_registry::model_vertex_data vertex_data;
	bool check_for_collisions;
	collider* coll;
	std::string last_frame_model_name;
	glm::vec3 velocity, acceleration;
	std::string collision_model_name;
	static std::vector<rigidbody*> rigidbodies;
};
template<typename _Ty> inline _Ty& rigidbody::set_collider_type() {
	delete this->coll;
	this->coll = new _Ty(this);
	return (_Ty&)*this->coll;
}
