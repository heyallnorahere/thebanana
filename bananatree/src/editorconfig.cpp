#include <thebanana.h>
#include "editorconfig.h"
#include <yaml-cpp/yaml.h>
namespace bananatree {
	void editorconfig::load(const std::string& path) {
		thebanana::g_game->debug_print("loading config from " + path);
		// load root node from a yaml file
		YAML::Node node = YAML::LoadFile(path);
		for (auto n : node) {
			// for every map, get the key, value, and type. unfortunately, i had to use very tedious code to figure out the type, but it works, so who cares
			std::string key = n["key"].as<std::string>();
			int type = n["type"].as<int>();
			switch (type) {
			// int
			case 0:
				this->set<int>(key, n["value"].as<int>());
				break;
			// unsigned int
			case 1:
				this->set<unsigned int>(key, n["value"].as<unsigned int>());
				break;
			// char
			case 2:
				this->set<char>(key, n["value"].as<char>());
				break;
			// unsigned char
			case 3:
				this->set<unsigned char>(key, n["value"].as<unsigned char>());
				break;
			// 64-bit int (long long)
			case 4:
				this->set<int64_t>(key, n["value"].as<int64_t>());
				break;
			// unsigned 64-bit int
			case 5:
				this->set<uint64_t>(key, n["value"].as<uint64_t>());
				break;
			// string
			case 6:
				this->set<std::string>(key, n["value"].as<std::string>());
				break;
			}
		}
	}
	void editorconfig::save(const std::string& path) {
		thebanana::g_game->debug_print("saving config to " + path);
		// create yaml emitter
		YAML::Emitter out;
		// begin a sequence of maps
		out << YAML::BeginSeq;
		for (auto& p : this->m_properties) {
			// begin a map containing the details of the property
			out << YAML::BeginMap;
			int type;
			size_t hash_code = p.second->get_type_hash();
			out << YAML::Key << "key" << YAML::Value << p.first;
			out << YAML::Key << "value" << YAML::Value;
			if (hash_code == typeid(int).hash_code()) {
				out << this->get<int>(p.first);
				// int: 0
				type = 0;
			} else if (hash_code == typeid(unsigned int).hash_code()) {
				out << this->get<unsigned int>(p.first);
				// unsigned int: 1
				type = 1;
			} else if (hash_code == typeid(char).hash_code()) {
				out << this->get<char>(p.first);
				// char: 2
				type = 2;
			} else if (hash_code == typeid(unsigned char).hash_code()) {
				out << this->get<unsigned char>(p.first);
				// unsigned char: 3
				type = 3;
			} else if (hash_code == typeid(int64_t).hash_code()) {
				out << this->get<int64_t>(p.first);
				// 64-bit int: 4
				type = 4;
			} else if (hash_code == typeid(uint64_t).hash_code()) {
				out << this->get<uint64_t>(p.first);
				// unsigned 64-bit int: 5
				type = 5;
			} else if (hash_code == typeid(std::string).hash_code()) {
				out << this->get<std::string>(p.first);
				// string: 6
				type = 6;
			}
			else assert(false);
			out << YAML::Key << "type" << YAML::Value << type;
			// end the map
			out << YAML::EndMap;
		}
		// end the sequence
		out << YAML::EndSeq;
		// print to a file
		std::ofstream fout(path);
		fout << out.c_str();
		fout.close();
	}
	void editorconfig::restore_defaults() {
		this->clear_map();
		// restore the default config properties
		this->set<std::string>("sandboxexe", "..\\x64\\$(Configuration)\\sandbox.exe");
		this->set<std::string>("msbuild", "C:\\Program Files (x86)\\Microsoft Visual Studio\\2019\\Community\\MSBuild\\Current\\Bin\\msbuild.exe");
		this->set<std::string>("sandboxworkdir", "..\\sandbox");
	}
	editorconfig::~editorconfig() {
		this->clear_map();
	}
	void editorconfig::clear_map() {
		// clear all of the properties
		for (auto& p : this->m_properties) {
			delete p.second;
		}
		this->m_properties.clear();
	}
	editorconfig::configproperty_base::~configproperty_base() { }
}