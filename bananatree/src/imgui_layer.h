#pragma once
#include "panels/panel.h"
namespace bananatree {
	class imgui_layer {
	public:
		imgui_layer();
		~imgui_layer();
		void update();
		void render();
		template<typename T> T* add_panel();
		template<typename T> T* find_panel();
		panel* get_panel(size_t index);
		size_t get_panel_count() const;
	private:
		void begin();
		void end();
		void start_dockspace();
		void set_theme();
		std::vector<std::unique_ptr<panel>> m_panels;
		bool m_static_mesh_creation_window_open;
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