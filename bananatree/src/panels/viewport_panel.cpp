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
		ImGui::Begin("Viewport", &this->m_open);
		ImVec2 content_region = ImGui::GetContentRegionAvail();
		this->m_framebuffer->bind();
		if (content_region.x > 0.f && content_region.y > 0.f) glViewport(0, 0, content_region.x, content_region.y);
		thebanana::g_game->render();
		this->m_framebuffer->unbind();
		auto att = this->m_framebuffer->get_attachments();
		void* texture = att[this->m_framebuffer->get_attachment_map().color_index].value;
		ImGui::Image(texture, content_region, ImVec2(0.f, 1.f), ImVec2(1.f, 0.f));
		ImGui::End();
	}
	std::string viewport_panel::get_menu_text() {
		return "Viewport";
	}
}