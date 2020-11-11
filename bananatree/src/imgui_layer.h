#pragma once
#include "panels/panel.h"
namespace bananatree {
	class imgui_layer {
	public:
		imgui_layer();
		~imgui_layer();
		void update();
		void render();
		void add_panel(panel* p);
		panel* get_panel(size_t index);
		size_t get_panel_count() const;
	private:
		void begin();
		void end();
		void start_dockspace();
		std::vector<std::unique_ptr<panel>> m_panels;
	};
}