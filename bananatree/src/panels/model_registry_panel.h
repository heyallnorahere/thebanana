#pragma once
#include "panel.h"
namespace bananatree {
	class model_registry_panel : public panel {
	public:
		model_registry_panel();
		virtual void render() override;
		virtual std::string get_menu_text() override;
		void import(const std::string& path, const std::string& name);
	private:
		bool m_import_menu_open;
	};
}