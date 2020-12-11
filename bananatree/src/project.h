#pragma once
namespace bananatree {
	class editor_layer;
	class project {
	public:
		struct model_descriptor {
			std::string path, name;
			bool should_replace;
			std::string find, replace;
		};
		struct material_descriptor {
			std::string friendly_name;
			std::string image_path;
			glm::vec3 diffuse, specular, ambient;
			float shininess;
			unsigned long long uuid;
		};
		project();
		void set_editor_layer(editor_layer* il);
		editor_layer* get_editor_layer();
		void rename(const std::string& name);
		void reset();
		void save(const std::string& path);
		void save();
		void load(const std::string& path);
		std::string get_name();
		std::string get_main_scene_path();
		std::string get_code_project_path();
		std::string get_dll_name();
		void set_main_scene_path(const std::string& path);
		void set_code_project_path(const std::string& path);
		void set_dll_name(const std::string& name);
		void register_model(const model_descriptor& md);
		bool has_temp_path();
		const std::vector<model_descriptor>& get_descriptors();
		std::string get_path();
		material_descriptor* add_material();
		const std::vector<material_descriptor>& get_materials();
	private:
		std::string m_name, m_main_scene, m_code_project, m_dll_name;
		std::vector<model_descriptor> m_descriptors;
		std::vector<material_descriptor> m_materials;
		editor_layer* m_editor_layer;
		std::string m_temp_path;
	};
}