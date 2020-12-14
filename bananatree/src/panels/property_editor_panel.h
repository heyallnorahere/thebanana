#pragma once
#include "panel.h"
namespace bananatree {
	class scene_hierarchy_panel;
	class texture_viewer_panel;
	class property_editor_panel : public panel {
	public:
		property_editor_panel();
		virtual void render() override;
		virtual std::string get_menu_text() override;
		void set_hierarchy(scene_hierarchy_panel* hierarchy);
		void set_texture_viewer(texture_viewer_panel* viewer);
	private:
		scene_hierarchy_panel* m_hierarchy;
		texture_viewer_panel* m_texture_viewer;
		int m_component_index;
	};
}