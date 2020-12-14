#include <thebanana.h>
#include "texture_viewer_panel.h"
#include <imgui.h>
#include <backends/imgui_impl_win32.h>
#include <backends/imgui_impl_opengl3.h>
#include <misc/cpp/imgui_stdlib.h>
namespace bananatree {
	texture_viewer_panel::texture_viewer_panel() {
		this->m_texture = -1;
	}
	void texture_viewer_panel::render() {
		ImGui::Begin("Texture viewer", &this->m_open);
		if (this->m_texture > -1) {
			ImGui::Image((void*)this->m_texture, ImGui::GetContentRegionAvail(), ImVec2(0.f, 1.f), ImVec2(1.f, 0.f));
		}
		else {
			ImGui::Text("There is no texture selected.");
		}
		ImGui::End();
	}
	std::string texture_viewer_panel::get_menu_text() {
		return "Texture viewer";
	}
	void texture_viewer_panel::set_texture(int texture) {
		this->m_texture = texture;
	}
	int texture_viewer_panel::get_texture() {
		return this->m_texture;
	}
}