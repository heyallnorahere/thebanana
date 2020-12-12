#pragma once
#include "panel.h"
#include "../project.h"
namespace bananatree {
	class material_editor_panel : public panel {
	public:
		void refresh();
		virtual void render() override;
		virtual std::string get_menu_text() override;
		void set_project(const std::shared_ptr<project>& p);
		void add_material_desc(thebanana::material* mat);
		std::vector<project::material_descriptor> get_descriptors();
	private:
		std::vector<thebanana::material*> m_materials;
		std::vector<project::material_descriptor> m_descriptors;
		std::shared_ptr<project> m_project;
		std::shared_ptr<thebanana::graphics::texture> m_material_texture_ptr, m_material_normal_map_ptr;
		int m_index;
	};
}