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
	virtual void clean_up() override;
	template<typename _Ty> void set_collider_type();
	gameobject* get_parent();
	model_registry::model_vertex_data get_vertex_data();
private:
	model_registry::model_vertex_data vertex_data;
	bool is_player;
	collider* coll;
	std::string last_frame_model_name;
};
template<typename _Ty> inline void rigidbody::set_collider_type() {
	delete this->coll;
	this->coll = new _Ty(this);
}
