#pragma once
#include "panel.h"
namespace bananatree {
	class script_registry_panel : public panel {
	public:
		script_registry_panel();
		virtual void render() override;
		virtual std::string get_menu_text() override;
		void refresh();
	private:
		std::vector<std::string> m_scripts;
	};
}