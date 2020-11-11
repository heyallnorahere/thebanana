#include <thebanana.h>
#include "editor_layer.h"
#include "../resource.h"
namespace bananatree {
	void editor_layer::init() {
		this->m_imgui_layer = std::shared_ptr<imgui_layer>(new imgui_layer());
		thebanana::g_game->show_cursor();
		thebanana::g_game->unclip_cursor();
		thebanana::g_game->get_shader_registry()->register_shader("basic", new opengl_shader_library::win32_resource_shader(IDR_BASIC_VERTEX, IDR_BASIC_FRAGMENT));
		thebanana::g_game->get_scene()->set_shader_name("basic");
	}
	void editor_layer::gameloop() {
		this->m_imgui_layer->update();
		thebanana::g_game->clear_screen();
		this->m_imgui_layer->render();
		thebanana::g_game->swap_buffers();
	}
	std::string editor_layer::window_title() {
		return "the banana tree";
	}
}