#pragma once
#include "panel.h"
namespace bananatree {
	class scene_hierarchy_panel : public panel {
	public:
		scene_hierarchy_panel();
		virtual void render() override;
		virtual std::string get_menu_text() override;
		thebanana::gameobject* get_selected_object();
		void set_selected_object(thebanana::gameobject* object);
	private:
		void tree_helper(thebanana::gameobject* object);
		void tree(thebanana::gameobject* object);
		bool gameobject_context_menu(thebanana::gameobject* object);
		bool m_open_static_mesh_menu;
		thebanana::gameobject* m_current_static_mesh_parent;
	};
}