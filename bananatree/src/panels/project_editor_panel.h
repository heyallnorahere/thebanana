#pragma once
#include "panel.h"
namespace bananatree {
	class project;
	class project_editor_panel : public panel {
	public:
		virtual void render() override;
		virtual std::string get_menu_text() override;
		void set_project(const std::shared_ptr<project>& p);
		void set_current_name(const std::string& name);
	private:
		std::shared_ptr<project> m_project;
		std::string m_current_name;
	};
}