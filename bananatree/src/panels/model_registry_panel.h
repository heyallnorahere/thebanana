#pragma once
#include "panel.h"
#include "../project.h"
namespace bananatree {
	class model_registry_panel : public panel {
	public:
		model_registry_panel();
		virtual void render() override;
		virtual std::string get_menu_text() override;
		void import(const std::string& path, const std::string& name);
		void import(const project::model_descriptor& md);
		void set_project(const std::shared_ptr<project>& p);
	private:
		bool m_import_menu_open;
		std::shared_ptr<project> m_project;
	};
}