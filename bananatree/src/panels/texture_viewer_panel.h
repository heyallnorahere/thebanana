#pragma once
#include "panel.h"
namespace bananatree {
	class texture_viewer_panel : public panel {
	public:
		texture_viewer_panel();
		virtual void render() override;
		virtual std::string get_menu_text() override;
		void set_texture(int texture);
		int get_texture();
	private:
		int m_texture;
	};
}