#pragma once
class rigidbody;
class collider {
public:
	collider(rigidbody* rb);
	virtual ~collider();
	virtual void detect_collision(rigidbody* other) = 0;
protected:
	rigidbody* parent;
};
class sphere_collider : public collider {
public:
	sphere_collider(rigidbody* rb);
	virtual void detect_collision(rigidbody* other) override;
private:
	glm::vec3 origin_offset;
	float radius;
};