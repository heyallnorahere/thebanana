#pragma once
#include "banana_api.h"
namespace thebanana {
	class rigidbody;
	class gameobject;
	class collider {
	public:
		BANANA_API collider(rigidbody* rb);
		BANANA_API virtual ~collider();
		virtual bool detect_collision(rigidbody* other) = 0;
	protected:
		rigidbody* parent;
	};
	//https://gist.github.com/mlfarrell/5b1d77326fb6f95c4fa7d9cf8622e992
	class mlfarrel_model : public collider {
	public:
		BANANA_API mlfarrel_model(rigidbody* rb);
		BANANA_API virtual bool detect_collision(rigidbody* other) override;
		BANANA_API mlfarrel_model& set_radius(float radius);
		BANANA_API mlfarrel_model& set_origin_offset(glm::vec3 offset);
		BANANA_API float get_radius() const;
		BANANA_API glm::vec3 get_origin_offset() const;
	private:
		glm::vec3 origin_offset;
		glm::vec3 last_frame_position;
		float radius;
	};
	// my own implementation
	class rectangular_prism_collider : public collider {
	public:
		BANANA_API rectangular_prism_collider(rigidbody* rb);
		BANANA_API virtual bool detect_collision(rigidbody* other) override;
		BANANA_API rectangular_prism_collider& set_dimensions(glm::vec3 dimensions);
		BANANA_API rectangular_prism_collider& set_origin_offset(glm::vec3 offset);
		BANANA_API glm::vec3 get_dimensions() const;
		BANANA_API glm::vec3 get_origin_offset() const;
	private:
		glm::vec3 dimensions, origin_offset;
	};
}