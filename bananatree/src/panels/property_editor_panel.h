#pragma once
#include "panel.h"
namespace bananatree {
	class scene_hierarchy_panel;
	class property_editor_panel : public panel {
	public:
		property_editor_panel();
		virtual void render() override;
		virtual std::string get_menu_text() override;
		void set_hierarchy(scene_hierarchy_panel* hierarchy);
	private:
		scene_hierarchy_panel* m_hierarchy;
		bool m_show_transform_menu;
		int m_component_index;
	};
}