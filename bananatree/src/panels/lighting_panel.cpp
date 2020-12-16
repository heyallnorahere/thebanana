#include <thebanana.h>
#include "lighting_panel.h"
#include <imgui.h>
#include <backends/imgui_impl_win32.h>
#include <backends/imgui_impl_opengl3.h>
#include <misc/cpp/imgui_stdlib.h>
namespace bananatree {
	void lighting_panel::render() {
		ImGui::Begin("Lighting", &this->m_open);
		if (ImGui::CollapsingHeader("Shadow config")) {
			thebanana::scene::shadow_settings& settings = thebanana::g_game->get_scene()->get_shadow_settings();
			float bounds = fabs(settings.left) + fabs(settings.right) + fabs(settings.bottom) + fabs(settings.top);
			bounds /= 4.f;
			ImGui::DragFloat("View frustum bounds", &bounds);
			settings.left = -bounds;
			settings.right = bounds;
			settings.bottom = -bounds;
			settings.top = bounds;
			ImGui::DragFloat("Near plane", &settings.near_plane);
			ImGui::DragFloat("Far plane", &settings.far_plane);
		}
		ImGui::End();
	}
	std::string lighting_panel::get_menu_text() {
		return "Lighting";
	}
}