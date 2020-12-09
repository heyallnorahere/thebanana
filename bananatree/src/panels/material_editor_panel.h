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
		void add_material(const project::material_descriptor& md);
	private:
		void commit();
		std::vector<thebanana::material*> m_materials;
		struct matdesc {
			matdesc(project::material_descriptor* ptr) {
				this->ptr = ptr;
				this->copy = *this->ptr;
			}
			project::material_descriptor* ptr, copy;
		};
		std::vector<matdesc> m_descriptors;
		std::shared_ptr<project> m_project;
		std::shared_ptr<thebanana::graphics::texture> m_material_albedo_ptr;
		int m_index;
	};
}