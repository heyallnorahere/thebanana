#pragma once
#include "panel.h"
namespace bananatree {
	class scene_hierarchy_panel : public panel {
	public:
		virtual void render() override;
		virtual std::string get_menu_text() override;
		thebanana::gameobject* get_selected_object();
	private:
		void tree_helper(thebanana::gameobject* object);
		void tree(thebanana::gameobject* object);
		thebanana::gameobject* m_selected_object;
	};
}