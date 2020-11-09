#include "pch.h"
#include "scene_serializer.h"
#include "scene.h"
#include "gameobject.h"
#include "component.h"
#include "physics/physics.h"
#include "components/components.h"
#include "debug_tools.h"
namespace thebanana {
	YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec3& v) {
		out << YAML::Flow;
		out << YAML::BeginSeq << v.x << v.y << v.z << YAML::EndSeq;
		return out;
	}
	YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec4& v) {
		out << YAML::Flow;
		out << YAML::BeginSeq << v.x << v.y << v.z << v.w << YAML::EndSeq;
		return out;
	}
	scene_serializer::scene_serializer(scene* scene) : m_scene(scene) { }
	static void serialize_collider(YAML::Emitter& out, collider* c) {
		assert(c);
		out << YAML::Key << "collider" << YAML::BeginMap;
		size_t hash = typeid(*c).hash_code();
		if (hash == typeid(mlfarrel_model).hash_code()) {
			mlfarrel_model* c_ = (mlfarrel_model*)c;
			out << YAML::Key << "type" << YAML::Value << "mlfarrel_model";
			out << YAML::Key << "radius" << YAML::Value << c_->get_radius();
			out << YAML::Key << "origin_offset" << YAML::Value << c_->get_origin_offset();
		}
		out << YAML::EndMap;
	}
	static void serialize_object(YAML::Emitter& out, gameobject* object) {
		out << YAML::BeginMap;
		out << YAML::Key << "object" << YAML::Value << "352456345756"; // todo: add unique identifier
		out << YAML::Key << "nickname" << YAML::Value << object->get_nickname();
		out << YAML::Key << "transform" << YAML::Value << YAML::BeginSeq;
		glm::mat4 matrix = object->get_transform().get_matrix();
		out << matrix[0] << matrix[1] << matrix[2] << matrix[3] << YAML::EndSeq;
		out << YAML::Key << "components" << YAML::BeginMap;
		for (size_t i = 0; i < object->get_number_components<tag_component>(); i++) {
			std::stringstream ss;
			ss << "tag_component " << i;
			out << YAML::Key << ss.str() << YAML::BeginMap;
			tag_component& c = object->get_component<tag_component>(i);
			out << YAML::Key << "value" << YAML::Value << c.get_tag();
			out << YAML::EndMap;
		}
		for (size_t i = 0; i < object->get_number_components<mesh_component>(); i++) {
			std::stringstream ss;
			ss << "mesh_component " << i;
			out << YAML::Key << ss.str() << YAML::BeginMap;
			mesh_component& c = object->get_component<mesh_component>();
			out << YAML::Key << "mesh name" << YAML::Value << *(c.get_property<std::string>("mesh name"));
			out << YAML::EndMap;
		}
		for (size_t i = 0; i < object->get_number_components<animation_component>(); i++) {
			std::stringstream ss;
			ss << "animation_component " << i;
			out << YAML::Key << ss.str() << YAML::BeginMap;
			out << YAML::EndMap;
		}
		for (size_t i = 0; i < object->get_number_components<rigidbody>(); i++) {
			rigidbody& rb = object->get_component<rigidbody>();
			std::stringstream ss;
			ss << "rigidbody " << i;
			out << YAML::Key << ss.str() << YAML::BeginMap;
			out << YAML::Key << "gravity" << YAML::Value << *(rb.get_property<bool>("gravity"));
			out << YAML::Key << "gravity multiplier" << YAML::Value << *(rb.get_property<float>("gravity multiplier"));
			out << YAML::Key << "mass" << YAML::Value << *(rb.get_property<float>("mass"));
			out << YAML::Key << "drag" << YAML::Value << *(rb.get_property<float>("drag"));
			collider* c = rb.get_collider();
			if (c) serialize_collider(out, c);
			out << YAML::EndMap;
		}
		// todo: figure out how to serialize a native_script_component
		out << YAML::EndMap;
		out << YAML::Key << "children" << YAML::Value << YAML::BeginSeq;
		for (size_t i = 0; i < object->get_children_count(); i++) {
			serialize_object(out, object->get_child(i));
		}
		out << YAML::EndSeq;
		out << YAML::EndMap;
	}
	void scene_serializer::serialize(const std::string& path) {
		debug::log_print("saving scene to " + path);
		YAML::Emitter out;
		out << YAML::BeginMap;
		out << YAML::Key << "scene" << YAML::Value << "untitled";
		out << YAML::Key << "objects" << YAML::Value << YAML::BeginSeq;
		for (size_t i = 0; i < this->m_scene->get_children_count(); i++) {
			serialize_object(out, this->m_scene->get_child(i));
		}
		out << YAML::EndSeq;
		out << YAML::EndMap;
		std::ofstream fout(path);
		fout << out.c_str();
		fout.close();
	}
	static gameobject* deserialize_object(const YAML::Node& node) {
		return NULL;
	}
	void scene_serializer::deserialize(const std::string& path) {
		YAML::Node data = YAML::LoadFile(path);
		assert(data["scene"]);
		std::string name = data["scene"].as<std::string>();
		debug::log_print("loading scene " + name + " from " + path);
		auto objects = data["objects"];
		if (objects) {
			for (auto obj : objects) {
				gameobject* object = deserialize_object(obj);
				assert(object);
				this->m_scene->add_object(object);
			}
		}
	}
}