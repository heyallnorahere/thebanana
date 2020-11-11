#pragma once
#include "panel.h"
namespace bananatree {
	class viewport_panel : public panel {
	public:
		viewport_panel();
		virtual void render() override;
		virtual std::string get_menu_text() override;
	private:
		std::unique_ptr<thebanana::graphics::framebuffer> m_framebuffer;
	};
}