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
		thebanana::gameobject* m_selected_object;
	};
}