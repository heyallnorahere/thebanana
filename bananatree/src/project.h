#pragma once
namespace bananatree {
	class imgui_layer;
	class project {
	public:
		struct model_descriptor {
			std::string path, name;
			bool should_replace;
			std::string find, replace;
		};
		project();
		void set_imgui_layer(imgui_layer* il);
		void rename(const std::string& name);
		void reset();
		void save(const std::string& path);
		void save();
		void load(const std::string& path);
		std::string get_name();
		void register_model(const model_descriptor& md);
		bool has_temp_path();
		const std::vector<model_descriptor>& get_descriptors();
	private:
		std::string m_name;
		std::vector<model_descriptor> m_descriptors;
		imgui_layer* m_imgui_layer;
		std::string m_temp_path;
	};
}