#include <thebanana.h>
#include "viewport_panel.h"
#include <imgui.h>
#include <backends/imgui_impl_win32.h>
#include <backends/imgui_impl_opengl3.h>
#include <misc/cpp/imgui_stdlib.h>
namespace bananatree {
	viewport_panel::viewport_panel() {
		thebanana::graphics::opengl::opengl_framebuffer::desc desc;
		desc.width = 800;
		desc.height = 600;
		desc.buffers = thebanana::graphics::opengl::opengl_framebuffer::desc::color | thebanana::graphics::opengl::opengl_framebuffer::desc::depth;
		this->m_framebuffer = std::unique_ptr<thebanana::graphics::framebuffer>(thebanana::graphics::framebuffer::create(&desc));
	}
	void viewport_panel::render() {
		this->m_framebuffer->bind();
		thebanana::g_game->render();
		this->m_framebuffer->unbind();
		auto att = this->m_framebuffer->get_attachments();
		void* texture = att[this->m_framebuffer->get_attachment_map().color_index].value;
		ImGui::Begin("Viewport", &this->m_open);
		ImGui::Image(texture, ImVec2(ImGui::GetWindowWidth(), ImGui::GetWindowHeight()), ImVec2(0.f, 1.f), ImVec2(1.f, 0.f));
		ImGui::End();
	}
	std::string viewport_panel::get_menu_text() {
		return "Viewport";
	}
}