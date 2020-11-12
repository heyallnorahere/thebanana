#include <thebanana.h>
#include "project.h"
#include <yaml-cpp/yaml.h>
namespace bananatree {
	project::project() {
		this->reset();
	}
	void project::rename(const std::string& name) {
		this->m_name = name;
		std::string window_text = this->m_name + " - The Banana Tree";
		SetWindowTextA(thebanana::g_game->get_window(), window_text.c_str());
	}
	void project::reset() {
		this->rename("Untitled");
	}
	void project::save(const std::string& path) {
		YAML::Emitter out;
		out << YAML::BeginMap;
		out << YAML::Key << "name" << YAML::Value << this->m_name;
		out << YAML::EndMap;
		std::ofstream fout(path);
		fout << out.c_str();
		fout.close();
	}
	void project::load(const std::string& path) {
		YAML::Node file = YAML::LoadFile(path);
		assert(file["name"]);
		this->rename(file["name"].as<std::string>());
	}
}