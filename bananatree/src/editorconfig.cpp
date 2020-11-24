#include <thebanana.h>
#include "editorconfig.h"
#include <yaml-cpp/yaml.h>
namespace bananatree {
	void editorconfig::load(const std::string& path) {
		thebanana::debug::log_print("loading config from " + path);
		YAML::Node node = YAML::LoadFile(path);
		for (auto n : node) {
			std::string key = n["key"].as<std::string>();
			int type = n["type"].as<int>();
			switch (type) {
			case 0:
				this->set<int>(key, n["value"].as<int>());
				break;
			case 1:
				this->set<unsigned int>(key, n["value"].as<unsigned int>());
				break;
			case 2:
				this->set<char>(key, n["value"].as<char>());
				break;
			case 3:
				this->set<unsigned char>(key, n["value"].as<unsigned char>());
				break;
			case 4:
				this->set<int64_t>(key, n["value"].as<int64_t>());
				break;
			case 5:
				this->set<uint64_t>(key, n["value"].as<uint64_t>());
				break;
			case 6:
				this->set<std::string>(key, n["value"].as<std::string>());
				break;
			}
		}
	}
	void editorconfig::save(const std::string& path) {
		thebanana::debug::log_print("saving config to " + path);
		YAML::Emitter out;
		out << YAML::BeginSeq;
		for (auto& p : this->m_properties) {
			out << YAML::BeginMap;
			int type;
			size_t hash_code = p.second->get_type_hash();
			out << YAML::Key << "key" << YAML::Value << p.first;
			out << YAML::Key << "value" << YAML::Value;
			if (hash_code == typeid(int).hash_code()) {
				out << this->get<int>(p.first);
				type = 0;
			} else if (hash_code == typeid(unsigned int).hash_code()) {
				out << this->get<unsigned int>(p.first);
				type = 1;
			} else if (hash_code == typeid(char).hash_code()) {
				out << this->get<char>(p.first);
				type = 2;
			} else if (hash_code == typeid(unsigned char).hash_code()) {
				out << this->get<unsigned char>(p.first);
				type = 3;
			} else if (hash_code == typeid(int64_t).hash_code()) {
				out << this->get<int64_t>(p.first);
				type = 4;
			} else if (hash_code == typeid(uint64_t).hash_code()) {
				out << this->get<uint64_t>(p.first);
				type = 5;
			} else if (hash_code == typeid(std::string).hash_code()) {
				out << this->get<std::string>(p.first);
				type = 6;
			}
			else assert(false);
			out << YAML::Key << "type" << YAML::Value << type;
			out << YAML::EndMap;
		}
		out << YAML::EndSeq;
		std::ofstream fout(path);
		fout << out.c_str();
		fout.close();
	}
	void editorconfig::restore_defaults() {
		this->clear_map();
		this->set<std::string>("sandboxexe", "..\\x64\\$(Configuration)\\sandbox.exe");
		this->set<std::string>("msbuild", "C:\\Program Files (x86)\\Microsoft Visual Studio\\2019\\Community\\MSBuild\\Current\\Bin\\msbuild.exe");
	}
	editorconfig::~editorconfig() {
		this->clear_map();
	}
	void editorconfig::clear_map() {
		for (auto& p : this->m_properties) {
			delete p.second;
		}
		this->m_properties.clear();
	}
	editorconfig::configproperty_base::~configproperty_base() { }
}