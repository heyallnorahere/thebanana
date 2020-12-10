#pragma once
#include "component.h"
#include "model_registry.h"
#include "collider.h"
#include "scene_serializer.h"
#include "banana_api.h"
namespace thebanana {
	class rigidbody : public component {
	public:
		BANANA_API rigidbody(gameobject* obj);
		BANANA_API virtual void initialize() override;
		BANANA_API virtual void pre_update() override;
		BANANA_API virtual void post_update() override;
		BANANA_API virtual void on_collision(gameobject* other) override;
		BANANA_API virtual void clean_up() override;
		template<typename _Ty> _Ty* set_collider_type();
		BANANA_API rigidbody& set_check_for_collisions(bool check);
		BANANA_API bool is_checking_for_collisions();
		BANANA_API gameobject* get_parent();
		BANANA_API model_registry::model_vertex_data get_vertex_data();
		BANANA_API glm::vec3& get_shift_delta();
		BANANA_API int& get_num_collisions();
		BANANA_API void apply_force(const glm::vec3& force);
		BANANA_API rigidbody& set_collision_model_name(const std::string& model_name);
		BANANA_API std::string get_collision_model_name();
		BANANA_API rigidbody& set_speed_cap(float cap);
		BANANA_API float get_speed_cap();
		BANANA_API float get_last_move_speed();
		BANANA_API collider* get_collider() const;
	private:
		int num_collisions;
		glm::vec3 shift_delta;
		model_registry::model_vertex_data vertex_data;
		bool check_for_collisions;
		collider* coll;
		std::string last_frame_model_name;
		glm::vec3 velocity, acceleration;
		glm::vec3 last_frame_pos;
		float speed_cap;
		bool has_cap;
		friend class scene_serializer;
	};
	template<> inline collider* rigidbody::set_collider_type<collider>() {
		delete this->coll;
		this->coll = NULL;
		return this->coll;
	}
	template<typename _Ty> inline _Ty* rigidbody::set_collider_type() {
		delete this->coll;
		this->coll = new _Ty(this);
		return (_Ty*)this->coll;
	}
}