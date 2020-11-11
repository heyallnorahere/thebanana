#pragma once
#include "panel.h"
namespace bananatree {
	class log_panel : public panel {
	public:
		virtual void render() override;
		virtual std::string get_menu_text() override;
	};
}