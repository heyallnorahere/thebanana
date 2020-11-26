#include "pch.h"
#include "scene_serializer.h"
#include "scene.h"
#include "basic_gameobject.h"
#include "component.h"
#include "physics/physics.h"
#include "components/components.h"
#include "particlesystem/particlesystem.h"
#include "debug_tools.h"
#include "script_registry.h"
#include "game.h"
namespace YAML {
	template<> struct convert<glm::vec2> {
		static Node encode(const glm::vec2& rhs) {
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			node.SetStyle(EmitterStyle::Flow);
			return node;
		}
		static bool decode(const Node& node, glm::vec2& rhs) {
			if (!node.IsSequence() || node.size() != 2) return false;
			rhs.x = node[0].as<float>();
			rhs.y = node[1].as<float>();
			return true;
		}
	};
	template<> struct convert<glm::vec3> {
		static Node encode(const glm::vec3& rhs) {
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			node.push_back(rhs.z);
			node.SetStyle(EmitterStyle::Flow);
			return node;
		}
		static bool decode(const Node& node, glm::vec3& rhs) {
			if (!node.IsSequence() || node.size() != 3) return false;
			rhs.x = node[0].as<float>();
			rhs.y = node[1].as<float>();
			rhs.z = node[2].as<float>();
			return true;
		}
	};
	template<> struct convert<glm::vec4> {
		static Node encode(const glm::vec4& rhs) {
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			node.push_back(rhs.z);
			node.push_back(rhs.w);
			node.SetStyle(EmitterStyle::Flow);
			return node;
		}
		static bool decode(const Node& node, glm::vec4& rhs) {
			if (!node.IsSequence() || node.size() != 4) return false;
			rhs.x = node[0].as<float>();
			rhs.y = node[1].as<float>();
			rhs.z = node[2].as<float>();
			rhs.w = node[3].as<float>();
			return true;
		}
	};
}
namespace thebanana {
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
		out << YAML::Key << "object" << YAML::Value << object->get_uuid();
		out << YAML::Key << "nickname" << YAML::Value << object->get_nickname();
		out << YAML::Key << "transform" << YAML::Value << YAML::BeginSeq;
		glm::mat4 matrix = object->get_transform().get_matrix();
		out << matrix[0] << matrix[1] << matrix[2] << matrix[3] << YAML::EndSeq;
		out << YAML::Key << "components" << YAML::Value << YAML::BeginSeq;
		for (size_t i = 0; i < object->get_number_components<tag_component>(); i++) {
			tag_component& c = object->get_component<tag_component>(i);
			out << YAML::BeginMap;
			out << YAML::Key << "type" << YAML::Value << "tag_component";
			out << YAML::Key << "uuid" << YAML::Value << c.get_uuid();
			out << YAML::Key << "value" << YAML::Value << c.get_tag();
			out << YAML::EndMap;
		}
		for (size_t i = 0; i < object->get_number_components<mesh_component>(); i++) {
			mesh_component& c = object->get_component<mesh_component>(i);
			out << YAML::BeginMap;
			out << YAML::Key << "type" << YAML::Value << "mesh_component";
			out << YAML::Key << "uuid" << YAML::Value << c.get_uuid();
			out << YAML::Key << "mesh name" << YAML::Value << *(c.get_property<std::string>("Mesh name"));
			out << YAML::EndMap;
		}
		for (size_t i = 0; i < object->get_number_components<animation_component>(); i++) {
			animation_component& c = object->get_component<animation_component>(i);
			out << YAML::BeginMap;
			out << YAML::Key << "type" << YAML::Value << "animation_component";
			out << YAML::Key << "uuid" << YAML::Value << c.get_uuid();
			out << YAML::EndMap;
		}
		for (size_t i = 0; i < object->get_number_components<rigidbody>(); i++) {
			rigidbody& rb = object->get_component<rigidbody>(i);
			out << YAML::BeginMap;
			out << YAML::Key << "type" << YAML::Value << "rigidbody";
			out << YAML::Key << "uuid" << YAML::Value << rb.get_uuid();
			out << YAML::Key << "gravity" << YAML::Value << *(rb.get_property<bool>("Gravity"));
			out << YAML::Key << "gravity multiplier" << YAML::Value << *(rb.get_property<float>("Gravity multiplier"));
			out << YAML::Key << "mass" << YAML::Value << *(rb.get_property<float>("Mass"));
			out << YAML::Key << "drag" << YAML::Value << *(rb.get_property<float>("Drag"));
			out << YAML::Key << "collision_model_name" << YAML::Value << rb.get_collision_model_name();
			out << YAML::Key << "check_for_collisions" << YAML::Value << rb.is_checking_for_collisions();
			collider* c = rb.get_collider();
			if (c) serialize_collider(out, c);
			out << YAML::EndMap;
		}
		for (size_t i = 0; i < object->get_number_components<camera_component>(); i++) {
			camera_component& cc = object->get_component<camera_component>(i);
			out << YAML::BeginMap;
			out << YAML::Key << "type" << YAML::Value << "camera_component";
			out << YAML::Key << "uuid" << YAML::Value << cc.get_uuid();
			out << YAML::Key << "angle" << YAML::Value << *cc.get_property<glm::vec2>("Angle");
			out << YAML::Key << "direction" << YAML::Value << *cc.get_property<glm::vec3>("Direction");
			out << YAML::EndMap;
		}
		for (size_t i = 0; i < object->get_number_components<particlesystem::particlesystem_component>(); i++) {
			particlesystem::particlesystem_component& psc = object->get_component<particlesystem::particlesystem_component>(i);
			out << YAML::BeginMap;
			out << YAML::Key << "type" << YAML::Value << "particlesystem_component";
			out << YAML::Key << "uuid" << YAML::Value << psc.get_uuid();
			out << YAML::Key << "spawn_interval" << YAML::Value << *(psc.get_property<float>("Spawn interval"));
			out << YAML::Key << "lifespan" << YAML::Value << *(psc.get_property<float>("Particle lifespan"));
			out << YAML::Key << "mesh_name" << YAML::Value << *(psc.get_property<std::string>("Particle model name"));
			out << YAML::Key << "force" << YAML::Value << *(psc.get_property<glm::vec3>("Particle acceleration"));
			out << YAML::Key << "min_size" << YAML::Value << *(psc.get_property<float>("Minimum particle size"));
			out << YAML::Key << "max_size" << YAML::Value << *(psc.get_property<float>("Maximum particle size"));
			out << YAML::EndMap;
		}
		for (size_t i = 0; i < object->get_number_components<native_script_component>(); i++) {
			native_script_component& nsc = object->get_component<native_script_component>(i);
			out << YAML::BeginMap;
			out << YAML::Key << "type" << YAML::Value << "native_script_component";
			out << YAML::Key << "uuid" << YAML::Value << nsc.get_uuid();
			out << YAML::Key << "script" << YAML::Value << nsc.get_property<component::property_base::read_only_text>("Script")->get_text();
			out << YAML::Key << "script_properties" << YAML::BeginSeq;
			for (size_t i = 1; i < nsc.get_properties().size(); i++) {
				auto it = nsc.get_properties().begin();
				std::advance(it, i);
				auto& prop = *it;
				out << YAML::BeginMap;
				out << YAML::Key << "name" << YAML::Value << prop->get_name();
				out << YAML::Key << "value" << YAML::Value;
				prop->send_to_yaml(out);
				out << YAML::Key << "type" << YAML::Value << prop->get_type_name();
				out << YAML::EndMap;
			}
			out << YAML::EndSeq;
			out << YAML::EndMap;
		}
		out << YAML::EndSeq;
		out << YAML::Key << "children" << YAML::Value << YAML::BeginSeq;
		for (size_t i = 0; i < object->get_children_count(); i++) {
			serialize_object(out, object->get_child(i));
		}
		out << YAML::EndSeq;
		out << YAML::EndMap;
		char uuidbuf[256];
		_ui64toa(object->get_uuid(), uuidbuf, 10);
		g_game->debug_print("serialized object\nnickname: " + object->get_nickname() + "\nUUID: " + uuidbuf);
	}
	void scene_serializer::serialize(const std::string& path) {
		this->m_scene->get_game()->debug_print("saving scene to " + path);
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
		this->m_scene->get_game()->debug_print("saved scene to " + path);
	}
	static void deserialize_collider(const YAML::Node& node, rigidbody& rb) {
		assert(node["type"]);
		std::string type = node["type"].as<std::string>();
		if (type == "mlfarrel_model") {
			mlfarrel_model& c = rb.set_collider_type<mlfarrel_model>();
			assert(node["radius"]);
			c.set_radius(node["radius"].as<float>());
			assert(node["origin_offset"]);
			c.set_origin_offset(node["origin_offset"].as<glm::vec3>());
		}
	}
	struct find_struct {
		unsigned long long uuid;
		gameobject** ptr;
	};
	std::vector<find_struct> to_find;
	static void deserialize_components(const YAML::Node& node, gameobject* object, scene* s) {
		assert(node);
		for (auto n : node) {
			assert(n["uuid"]);
			unsigned long long uuid = n["uuid"].as<unsigned long long>();
			bool has_same_uuid = false;
			for (size_t i = 0; i < object->get_number_components<component>(); i++) {
				component& c = object->get_component<component>(i);
				if (c.get_uuid() == uuid) {
					has_same_uuid = true;
					break;
				}
			}
			if (has_same_uuid) continue;
			assert(n["type"]);
			std::string type = n["type"].as<std::string>();
			if (type == "tag_component") {
				tag_component& tc = object->add_component<tag_component>();
				tc.set_uuid(uuid);
				assert(n["value"]);
				tc.set_tag(n["value"].as<std::string>());
			} else if (type == "mesh_component") {
				mesh_component& mc = object->add_component<mesh_component>();
				mc.set_uuid(uuid);
				assert(n["mesh name"]);
				mc.set_property<std::string>("Mesh name", n["mesh name"].as<std::string>());
			} else if (type == "animation_component") {
				animation_component& ac = object->add_component<animation_component>();
				ac.set_uuid(uuid);
			} else if (type == "rigidbody") {
				rigidbody& rb = object->add_component<rigidbody>();
				rb.set_uuid(uuid);
				assert(n["gravity"]);
				rb.set_property<bool>("Gravity", n["gravity"].as<bool>());
				rb.set_property<float>("Gravity multiplier", n["gravity multiplier"].as<float>());
				rb.set_property<float>("Mass", n["mass"].as<float>());
				rb.set_property<float>("Drag", n["drag"].as<float>());
				rb.set_collision_model_name(n["collision_model_name"].as<std::string>());
				rb.set_check_for_collisions(n["check_for_collisions"].as<bool>());
				if (n["collider"])
					deserialize_collider(n["collider"], rb);
			} else if (type == "camera_component") {
				camera_component& cc = object->add_component<camera_component>();
				cc.set_uuid(uuid);
				cc.set_property<glm::vec2>("Angle", n["angle"].as<glm::vec2>());
				cc.set_property<glm::vec3>("Direction", n["direction"].as<glm::vec3>());
			} else if (type == "particlesystem_component") {
				particlesystem::particlesystem_component& psc = object->add_component<particlesystem::particlesystem_component>();
				psc.set_uuid(uuid);
				psc.set_property("Spawn interval", n["spawn_interval"].as<float>());
				psc.set_property("Particle lifespan", n["lifespan"].as<float>());
				psc.set_property("Particle model name", n["mesh_name"].as<std::string>());
				psc.set_property("Particle acceleration", n["force"].as<glm::vec3>());
				psc.set_property("Minimum particle size", n["min_size"].as<float>());
				psc.set_property("Maximum particle size", n["max_size"].as<float>());
			} else if (type == "native_script_component") {
				native_script_component& nsc = object->add_component<native_script_component>();
				nsc.set_uuid(uuid);
				assert(n["script"]);
				std::string script_name = n["script"].as<std::string>();
				nsc.bind(s->get_game()->get_script_registry()->create_script(script_name, object, &nsc));
				nsc.set_property("Script", script_name);
				assert(n["script_properties"]);
				for (auto p : n["script_properties"]) {
					enum class property_type {
						i,
						b,
						f,
						d,
						s,
						v2,
						v3,
						v4,
						read_only,
						dropdown,
						object
					};
					property_type pt;
					assert(p["type"]);
					std::string type = p["type"].as<std::string>();
					if (type == typeid(int).name()) pt = property_type::i;
					else if (type == typeid(bool).name()) pt = property_type::b;
					else if (type == typeid(float).name()) pt = property_type::f;
					else if (type == typeid(double).name()) pt = property_type::d;
					else if (type == typeid(std::string).name()) pt = property_type::s;
					else if (type == typeid(glm::vec2).name()) pt = property_type::v2;
					else if (type == typeid(glm::vec3).name()) pt = property_type::v3;
					else if (type == typeid(glm::vec4).name()) pt = property_type::v4;
					else if (type == typeid(component::property_base::read_only_text).name()) pt = property_type::read_only;
					else if (type == typeid(component::property_base::dropdown).name()) pt = property_type::dropdown;
					else if (type == typeid(gameobject*).name()) pt = property_type::object;
					else assert(false);
					assert(p["name"]);
					std::string name = p["name"].as<std::string>();
					YAML::Node value_node = p["value"];
					assert(value_node);
					switch (pt) {
					case property_type::i:
						nsc.set_property(name, value_node.as<int>());
						break;
					case property_type::b:
						nsc.set_property(name, value_node.as<bool>());
						break;
					case property_type::f:
						nsc.set_property(name, value_node.as<float>());
						break;
					case property_type::d:
						nsc.set_property(name, value_node.as<double>());
						break;
					case property_type::s:
						nsc.set_property(name, value_node.as<std::string>());
						break;
					case property_type::v2:
						nsc.set_property(name, value_node.as<glm::vec2>());
						break;
					case property_type::v3:
						nsc.set_property(name, value_node.as<glm::vec3>());
						break;
					case property_type::v4:
						nsc.set_property(name, value_node.as<glm::vec4>());
						break;
					case property_type::read_only:
						nsc.set_property(name, component::property_base::read_only_text(value_node.as<std::string>()));
						break;
					case property_type::dropdown:
					{
						int index = value_node["index"].as<int>();
						std::vector<std::string> items;
						for (auto item : value_node["items"]) {
							items.push_back(item.as<std::string>());
						}
						nsc.set_property(name, component::property_base::dropdown(items, index));
					}
						break;
					case property_type::object:
						to_find.push_back({ value_node.as<unsigned long long>(), nsc.get_property<gameobject*>(name) });
						break;
					}
				}
			}
		}
	}
	struct add_object_struct {
		gameobject* parent;
		gameobject* child;
	};
	std::vector<add_object_struct> to_add;
	static gameobject* deserialize_object(const YAML::Node& node, gameobject* parent, scene* s) {
		gameobject* object = new basic_gameobject;
		assert(object);
		if (parent) {
			to_add.push_back({ parent, object });
		}
		YAML::Node uuid_node = node["object"];
		assert(uuid_node);
		unsigned long long uuid = uuid_node.as<unsigned long long>();
		gameobject* o = s->find(uuid);
		if (o) {
			char uuidbuf[256];
			_ui64toa(uuid, uuidbuf, 10);
			g_game->debug_print("failed to load object; an object with the uuid of " + std::string(uuidbuf) + " already exists");
			delete object;
			return NULL;
		}
		object->set_uuid(uuid);
		YAML::Node nickname_node = node["nickname"];
		assert(nickname_node);
		object->get_nickname() = nickname_node.as<std::string>();
		YAML::Node transform_node = node["transform"];
		assert(transform_node);
		glm::mat4 t(1.f);
		for (size_t i = 0; i < 4; i++) {
			t[i] = transform_node[i].as<glm::vec4>();
		}
		object->get_transform() = transform(t);
		deserialize_components(node["components"], object, s);
		assert(node["children"]);
		for (auto n : node["children"]) {
			gameobject* obj = deserialize_object(n, object, s);
		}
		char uuidbuf[256];
		_ui64toa(object->get_uuid(), uuidbuf, 10);
		g_game->debug_print("deserialized object\nnickname: " + object->get_nickname() + "\nUUID: " + uuidbuf);
		return object;
	}
	void scene_serializer::deserialize(const std::string& path) {
		to_find.clear();
		to_add.clear();
		this->m_scene->clear();
		YAML::Node data = YAML::LoadFile(path);
		assert(data["scene"]);
		std::string name = data["scene"].as<std::string>();
		this->m_scene->get_game()->debug_print("loading scene " + name + " from " + path);
		auto objects = data["objects"];
		if (objects) {
			for (auto obj : objects) {
				gameobject* object = deserialize_object(obj, NULL, this->m_scene);
				if (object != NULL)
					this->m_scene->add_object(object);
			}
		}
		for (auto aos : to_add) {
			aos.parent->add_object(aos.child);
		}
		for (auto fs : to_find) {
			*fs.ptr = this->m_scene->find(fs.uuid);
		}
		this->m_scene->get_game()->debug_print("loaded scene " + name + " from " + path);
	}
}