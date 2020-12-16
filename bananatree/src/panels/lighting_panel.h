#pragma once
#include "panel.h"
namespace bananatree {
	class lighting_panel : public panel {
	public:
		virtual void render() override;
		virtual std::string get_menu_text() override;
	};
}