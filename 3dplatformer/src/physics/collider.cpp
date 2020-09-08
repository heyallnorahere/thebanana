#include "pch.h"
#include "collider.h"
#include "rigidbody.h"
#include "gameobject.h"
#include "debug_tools.h"
collider::collider(rigidbody* rb) : parent(rb) { }
collider::~collider() { }
mlfarrel_model::mlfarrel_model(rigidbody* rb) : collider(rb), origin_offset(glm::vec3(0.f, 0.5f, 0.f)), radius(0.5f) { }
//https://gamedev.stackexchange.com/questions/96459/fast-ray-sphere-collision-code
static bool intersectRaySegmentSphere(glm::vec3 o, glm::vec3 d, glm::vec3 so, float radius2, glm::vec3& ip) {
	float l = d.length();
	d /= l;
	glm::vec3 m = o - so;
	float b = glm::dot(m, d);
	float c = glm::dot(m, m) - radius2;
	if (c > 0.0f && b > 0.0f)
		return false;
	float discr = b * b - c;
	if (discr < 0.0f)
		return false;
	float t = -b - sqrtf(discr);
	if (t < 0.0f)
		t = 0.0f;
	ip = o + (d * t);
	if (t > l)
		return false;
	return true;
}
static bool left_of(const glm::vec2& a, const glm::vec2& b, const glm::vec2& p) {
	float area = 0.5f * (a.x * (b.y - p.y) + b.x * (p.y - a.y) + p.x * (a.y - b.y));
	return (area > 0.f);
}
static bool point_inside(const glm::vec2 poly[], int pcount, const glm::vec2& v) {
	for (int i = 0; i < pcount; i++) {
		int next = i;
		next++;
		if (next == pcount) {
			next = 0;
		}
		if (!left_of(poly[i], poly[next], v)) {
			return false;
		}
	}
	return true;
}
bool mlfarrel_model::detect_collision(rigidbody* other) {
	bool collided = false;
	using uint3 = glm::vec<3, unsigned int, glm::packed_highp>;
	glm::vec3 origin = glm::vec3(this->parent->get_parent()->get_transform().get_matrix() * glm::vec4(0.f, 0.f, 0.f, 1.f)) + this->origin_offset;
	float radius_2 = this->radius * this->radius;
	gameobject* object = other->get_parent();
	model_registry::model_vertex_data data = other->get_vertex_data();
	std::vector<glm::vec3> positions, normals;
	for (auto v : data.vertices) {
		positions.push_back(v.pos);
		normals.push_back(v.normal);
	}
	std::vector<unsigned int> inds = data.indices;
	size_t num_tris = inds.size() / 3;
	uint3* indices = (uint3*)inds.data();
	for (size_t i = 0; i < num_tris; i++) {
		bool outside_plane = false;
		bool outside_all_vertices = false;
		bool outside_all_edges = false;
		bool fully_inside_plane = false;
		glm::vec3 v0 = object->get_transform().get_matrix() * glm::vec4(positions[indices[i][0]], 1.f);
		glm::vec3 v1 = object->get_transform().get_matrix() * glm::vec4(positions[indices[i][1]], 1.f);
		glm::vec3 v2 = object->get_transform().get_matrix() * glm::vec4(positions[indices[i][2]], 1.f);
		glm::vec3 normal = glm::normalize(glm::mat3(glm::transpose(glm::inverse(object->get_transform().get_matrix()))) * normals[indices[i].x]);
		if (fabs(normal.y) > 0.1f) continue;
		float d = glm::dot(-((v0 + v1 + v2) / 3.f), normal);
		float ppd = glm::dot(normal, origin) + d;
		if (fabs(ppd) > this->radius) {
			outside_plane = true;
			continue;
		}
		bool outside_v0 = ((v0 - origin).length() * (v0 - origin).length() > radius_2);
		bool outside_v1 = ((v1 - origin).length() * (v1 - origin).length() > radius_2);
		bool outside_v2 = ((v2 - origin).length() * (v2 - origin).length() > radius_2);
		if (outside_v0 && outside_v1 && outside_v2) {
			outside_all_vertices = true;
		}
		glm::vec3 a = v1 - v0;
		glm::vec3 b = v2 - v1;
		glm::vec3 c = v0 - v2;
		glm::vec3 plane_x = glm::normalize(a);
		glm::vec3 plane_y = glm::normalize(glm::cross(normal, a));
		auto project_2d = [&](const glm::vec3& p) { return glm::vec2(glm::dot(p, plane_x), glm::dot(p, plane_y)); };
		glm::vec2 plane_pos_2d = project_2d(origin);
		glm::vec2 triangle_2d[3] = { project_2d(v0), project_2d(v1), project_2d(v2) };
		if (point_inside(triangle_2d, 3, plane_pos_2d)) {
			fully_inside_plane = true;
		}
		glm::vec3 ip;
		if (!intersectRaySegmentSphere(v0, a, origin, radius_2, ip) && !intersectRaySegmentSphere(v1, b, origin, radius_2, ip) && !intersectRaySegmentSphere(v2, c, origin, radius_2, ip)) {
			outside_all_edges = true;
		}
		if (outside_all_edges && outside_all_vertices && !fully_inside_plane) {
			continue;
		}
		log_print("collided with gameobject of type " + std::string(typeid(*other->get_parent()).name()));
		this->parent->get_num_collisions()++;
		this->parent->get_shift_delta() += normal * (this->radius - ppd);
		if (!collided) collided = true;
	}
	return collided;
}
void mlfarrel_model::on_collision(gameobject* other) {
	glm::vec3& shift_delta = this->parent->get_shift_delta();
	float last_walk_speed = this->parent->get_parent()->get_last_walk_speed();
	if (glm::length(shift_delta) > last_walk_speed) {
		shift_delta = glm::normalize(shift_delta);
		shift_delta *= last_walk_speed * 1.1f;
	}
	this->parent->get_parent()->get_transform().move(shift_delta);
}
mlfarrel_model& mlfarrel_model::set_radius(float radius) {
	this->radius = radius;
	return *this;
}
mlfarrel_model& mlfarrel_model::set_origin_offset(glm::vec3 offset) {
	this->origin_offset = offset;
	return *this;
}
