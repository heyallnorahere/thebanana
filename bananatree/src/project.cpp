#include <thebanana.h>
#include "project.h"
#include <yaml-cpp/yaml.h>
#include "imgui_layer.h"
#include "panels/model_registry_panel.h"
#include "panels/project_editor_panel.h"
namespace bananatree {
	project::project() {
		this->reset();
	}
	void project::set_imgui_layer(imgui_layer* il) {
		this->m_imgui_layer = il;
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
		if (this->m_imgui_layer) {
			project_editor_panel* pep = this->m_imgui_layer->find_panel<project_editor_panel>();
			pep->set_current_name(this->m_name);
			pep->set_current_main_scene(this->m_main_scene);
			pep->set_current_code_project(this->m_code_project);
		}
		this->m_descriptors.clear();
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
		this->m_imgui_layer->find_panel<project_editor_panel>()->set_current_name(this->m_name);
		if (file["main_scene_path"]) {
			std::string path = file["main_scene_path"].as<std::string>();
			this->m_main_scene = path;
			this->m_imgui_layer->find_panel<project_editor_panel>()->set_current_main_scene(this->m_main_scene);
		}
		if (file["code_project_path"]) {
			std::string path = file["code_project_path"].as<std::string>();
			this->m_code_project = path;
			this->m_imgui_layer->find_panel<project_editor_panel>()->set_current_code_project(this->m_code_project);
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
			this->m_imgui_layer->open_scene(directory + this->m_main_scene);
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
			this->m_imgui_layer->find_panel<model_registry_panel>()->import(md);
		}
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
	void project::set_main_scene_path(const std::string& path) {
		this->m_main_scene = path;
	}
	void project::set_code_project_path(const std::string& path) {
		this->m_code_project = path;
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
}