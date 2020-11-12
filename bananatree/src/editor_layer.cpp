#include <thebanana.h>
#include "editor_layer.h"
#include "../resource.h"
namespace bananatree {
	void editor_layer::init() {
		this->m_project = std::shared_ptr<project>(new project);
		this->m_imgui_layer = std::shared_ptr<imgui_layer>(new imgui_layer(this));
		this->m_project->set_imgui_layer(&(*this->m_imgui_layer));
		thebanana::g_game->show_cursor();
		thebanana::g_game->unclip_cursor();
		thebanana::g_game->get_shader_registry()->register_shader("basic", new opengl_shader_library::win32_resource_shader(IDR_BASIC_VERTEX, IDR_BASIC_FRAGMENT));
		thebanana::g_game->get_scene()->set_shader_name("basic");
	}
	void editor_layer::gameloop() {
		thebanana::g_game->update();
		this->m_imgui_layer->update();
		thebanana::g_game->clear_screen();
		this->m_imgui_layer->render();
		thebanana::g_game->swap_buffers();
	}
	std::string editor_layer::window_title() {
		return "The Banana Tree";
	}
	std::shared_ptr<project> editor_layer::get_project() {
		return this->m_project;
	}
}