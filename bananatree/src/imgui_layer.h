#pragma once
#include "panels/panel.h"
namespace bananatree {
	class editor_layer;
	class imgui_layer {
	public:
		imgui_layer(editor_layer* el);
		~imgui_layer();
		void update();
		void render();
		template<typename T> T* add_panel();
		template<typename T> T* find_panel();
		panel* get_panel(size_t index);
		size_t get_panel_count() const;
		void new_scene();
		void open_scene(const std::string& path);
		void save_scene(const std::string& path);
		void open_scene();
		void save_scene();
		void save_scene_from_temp();
	private:
		void begin();
		void end();
		void start_dockspace();
		void set_theme();
		void parse_inputs();
		void save_project_as();
		std::vector<std::unique_ptr<panel>> m_panels;
		bool m_static_mesh_creation_window_open;
		editor_layer* m_editor_layer;
		std::string m_temp_scene_path;
	};
	template<typename T> inline T* imgui_layer::add_panel() {
		T* ptr = new T;
		this->m_panels.push_back(std::unique_ptr<panel>(ptr));
		return ptr;
	}
	template<typename T> inline T* imgui_layer::find_panel() {
		if (typeid(T).hash_code() == typeid(panel).hash_code()) {
			auto& p = this->m_panels[0];
			return (T*)p.get();
		}
		T* ptr = NULL;
		for (auto& p : this->m_panels) {
			if (typeid(*p).hash_code() == typeid(T).hash_code()) ptr = (T*)p.get();
		}
		return ptr;
	}
}