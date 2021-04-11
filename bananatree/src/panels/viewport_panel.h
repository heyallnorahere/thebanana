#pragma once
#include "panel.h"
namespace bananatree {
	class imgui_layer;
	class viewport_panel : public panel {
	public:
		enum class gizmo_operation {
			translate = 0,
			rotate = 1,
			scale = 2,
			none,
		};
		enum class transformation_mode {
			local = 0,
			world = 1,
		};
		viewport_panel();
		~viewport_panel();
		virtual void render() override;
		virtual std::string get_menu_text() override;
		void set_imgui_layer(imgui_layer* il);
		void set_gizmo_operation(gizmo_operation op);
		gizmo_operation get_gizmo_operation();
		void set_transformation_mode(transformation_mode mode);
		transformation_mode get_transformation_mode();
        thebanana::graphics::framebuffer* get_framebuffer();
	private:
		std::unique_ptr<thebanana::graphics::framebuffer> m_framebuffer;
		imgui_layer* m_imgui_layer;
		size_t m_keyboard_index;
		gizmo_operation m_gizmo_operation;
		transformation_mode m_transformation_mode;
	};
}
