#include <thebanana.h>
#include "project.h"
#include <yaml-cpp/yaml.h>
#include "editor_layer.h"
#include "panels/model_registry_panel.h"
#include "panels/project_editor_panel.h"
#include "panels/material_editor_panel.h"
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
namespace bananatree {
	YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec2& v) {
		out << YAML::Flow;
		out << YAML::BeginSeq << v.x << v.y << YAML::EndSeq;
		return out;
	}
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
	project::project() {
		this->reset();
	}
	void project::set_editor_layer(editor_layer* il) {
		this->m_editor_layer = il;
	}
	editor_layer* project::get_editor_layer() {
		return this->m_editor_layer;
	}
	void project::rename(const std::string& name) {
		this->m_name = name;
		std::string window_text = this->m_name + " - The Banana Tree";
		SetWindowTextA(thebanana::g_game->get_window(), window_text.c_str());
	}
	void project::reset() {
		if (!this->m_temp_path.empty()) this->m_temp_path.clear();
		if (!this->m_main_scene.empty()) this->m_main_scene.clear();
		if (!this->m_code_project.empty()) this->m_code_project.clear();
		this->rename("Untitled");
		thebanana::g_game->get_material_registry()->clear();
		if (this->m_editor_layer) {
			project_editor_panel* pep = this->m_editor_layer->get_imgui_layer()->find_panel<project_editor_panel>();
			pep->set_current_name(this->m_name);
			pep->set_current_main_scene(this->m_main_scene);
			pep->set_current_code_project(this->m_code_project);
			material_editor_panel* mep = this->m_editor_layer->get_imgui_layer()->find_panel<material_editor_panel>();
			mep->refresh();
		}
		this->m_descriptors.clear();
		this->m_materials.clear();
	}
	void project::save(const std::string& path) {
		this->m_temp_path = path;
		YAML::Emitter out;
		out << YAML::BeginMap;
		out << YAML::Key << "name" << YAML::Value << this->m_name;
		if (!this->m_main_scene.empty()) {
			out << YAML::Key << "main_scene_path" << YAML::Value << this->m_main_scene;
		}
		if (!this->m_code_project.empty()) {
			out << YAML::Key << "code_project_path" << YAML::Value << this->m_code_project;
		}
		if (!this->m_dll_name.empty()) {
			out << YAML::Key << "dll_name" << YAML::Value << this->m_dll_name;
		}
		out << YAML::Key << "models" << YAML::Value << YAML::BeginSeq;
		for (auto md : this->m_descriptors) {
			out << YAML::BeginMap;
			out << YAML::Key << "path" << YAML::Value << md.path;
			out << YAML::Key << "name" << YAML::Value << md.name;
			out << YAML::Key << "should_replace" << YAML::Value << md.should_replace;
			if (md.should_replace) {
				out << YAML::Key << "find" << YAML::Value << md.find;
				out << YAML::Key << "replace" << YAML::Value << md.replace;
			}
			out << YAML::EndMap;
		}
		out << YAML::EndSeq;
		out << YAML::Key << "materials" << YAML::Value << YAML::BeginSeq;
		for (auto md : this->m_materials) {
			out << YAML::BeginMap;
			out << YAML::Key << "friendly name" << YAML::Value << md.friendly_name;
			out << YAML::Key << "albedo" << YAML::Value << md.image_path;
			out << YAML::Key << "diffuse" << YAML::Value << md.diffuse;
			out << YAML::Key << "specular" << YAML::Value << md.specular;
			out << YAML::Key << "ambient" << YAML::Value << md.ambient;
			out << YAML::Key << "shininess" << YAML::Value << md.shininess;
			out << YAML::Key << "uuid" << YAML::Value << md.uuid;
			out << YAML::EndMap;
		}
		out << YAML::EndSeq;
		out << YAML::EndMap;
		std::ofstream fout(path);
		fout << out.c_str();
		fout.close();
	}
	void project::save() {
		this->save(this->m_temp_path);
	}
	void project::load(const std::string& path) {
		this->reset();
		this->m_temp_path = path;
		YAML::Node file = YAML::LoadFile(path);
		assert(file["name"]);
		this->rename(file["name"].as<std::string>());
		this->m_editor_layer->get_imgui_layer()->find_panel<project_editor_panel>()->set_current_name(this->m_name);
		if (file["main_scene_path"]) {
			std::string path = file["main_scene_path"].as<std::string>();
			this->m_main_scene = path;
			this->m_editor_layer->get_imgui_layer()->find_panel<project_editor_panel>()->set_current_main_scene(this->m_main_scene);
		} else {
			this->m_editor_layer->get_imgui_layer()->find_panel<project_editor_panel>()->set_current_main_scene(std::string());
		}
		if (file["code_project_path"]) {
			std::string path = file["code_project_path"].as<std::string>();
			this->m_code_project = path;
			this->m_editor_layer->get_imgui_layer()->find_panel<project_editor_panel>()->set_current_code_project(this->m_code_project);
		} else {
			this->m_editor_layer->get_imgui_layer()->find_panel<project_editor_panel>()->set_current_code_project(std::string());
		}
		if (file["dll_name"]) {
			std::string path = file["dll_name"].as<std::string>();
			this->m_dll_name = path;
			this->m_editor_layer->get_imgui_layer()->find_panel<project_editor_panel>()->set_current_dll_name(this->m_dll_name);
		} else {
			this->m_editor_layer->get_imgui_layer()->find_panel<project_editor_panel>()->set_current_dll_name(std::string());
		}
		if (file["code_project_path"] && file["dll_name"]) {
			this->m_editor_layer->compile_scripts();
		}
		if (!this->m_main_scene.empty()) {
			std::string path = this->m_temp_path;
			size_t pos = std::string::npos;
			do {
				pos = path.find_first_of('/');
				if (pos != std::string::npos) {
					this->m_temp_path.replace(pos, 1, "\\");
				}
			} while (pos != std::string::npos);
			pos = path.find_last_of('\\');
			std::string directory;
			if (pos != std::string::npos) {
				directory = path.substr(0, pos + 1);
			}
			this->m_editor_layer->get_imgui_layer()->open_scene(directory + this->m_main_scene);
		}
		assert(file["models"]);
		for (auto m : file["models"]) {
			model_descriptor md;
			md.path = m["path"].as<std::string>();
			md.name = m["name"].as<std::string>();
			md.should_replace = m["should_replace"].as<bool>();
			if (md.should_replace) {
				md.find = m["find"].as<std::string>();
				md.replace = m["replace"].as<std::string>();
			}
			this->m_descriptors.push_back(md);
			this->m_editor_layer->get_imgui_layer()->find_panel<model_registry_panel>()->import(md);
		}
		assert(file["materials"]);
		for (auto m : file["materials"]) {
			material_descriptor md;
			md.friendly_name = m["friendly name"].as<std::string>();
			md.image_path = m["albedo"].as<std::string>();
			md.diffuse = m["diffuse"].as<glm::vec3>();
			md.specular = m["specular"].as<glm::vec3>();
			md.ambient = m["ambient"].as<glm::vec3>();
			md.shininess = m["shininess"].as<float>();
			md.uuid = m["uuid"].as<unsigned long long>();
			this->m_materials.push_back(md);
			this->m_editor_layer->get_imgui_layer()->find_panel<material_editor_panel>()->add_material(md);
		}
		this->m_editor_layer->get_imgui_layer()->find_panel<material_editor_panel>()->refresh();
	}
	std::string project::get_name() {
		return this->m_name;
	}
	std::string project::get_main_scene_path() {
		return this->m_main_scene;
	}
	std::string project::get_code_project_path() {
		return this->m_code_project;
	}
	std::string project::get_dll_name() {
		return this->m_dll_name;
	}
	void project::set_main_scene_path(const std::string& path) {
		this->m_main_scene = path;
	}
	void project::set_code_project_path(const std::string& path) {
		this->m_code_project = path;
	}
	void project::set_dll_name(const std::string& name) {
		this->m_dll_name = name;
	}
	void project::register_model(const model_descriptor& md) {
		this->m_descriptors.push_back(md);
	}
	bool project::has_temp_path() {
		return !this->m_temp_path.empty();
	}
	const std::vector<project::model_descriptor>& project::get_descriptors() {
		return this->m_descriptors;
	}
	std::string project::get_path() {
		return this->m_temp_path;
	}
	project::material_descriptor* project::add_material() {
		this->m_materials.push_back(material_descriptor());
		return &this->m_materials[this->m_materials.size() - 1];
	}
	const std::vector<project::material_descriptor>& project::get_materials() {
		return this->m_materials;
	}
}