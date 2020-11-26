#include <thebanana.h>
#include "script_registry_panel.h"
#include <imgui.h>
#include <backends/imgui_impl_win32.h>
#include <backends/imgui_impl_opengl3.h>
#include <misc/cpp/imgui_stdlib.h>
namespace bananatree {
	static void script_node(const std::string& name) {
		if (ImGui::TreeNodeEx(name.c_str(), ImGuiTreeNodeFlags_Leaf)) {
			if (ImGui::BeginDragDropSource()) {
				ImGui::SetDragDropPayload("SCRIPT_PAYLOAD", name.c_str(), name.length());
				ImGui::Text(name.c_str());
				ImGui::EndDragDropSource();
			}
			ImGui::TreePop();
		}
	}
	script_registry_panel::script_registry_panel() {
		this->refresh();
	}
	void script_registry_panel::render() {
		ImGui::Begin("Script registry", &this->m_open);
		if (ImGui::Button("Refresh")) {
			this->refresh();
		}
		if (ImGui::TreeNodeEx("Scripts", ImGuiTreeNodeFlags_Leaf)) {
			for (auto s : this->m_scripts) {
				script_node(s);
			}
			ImGui::TreePop();
		}
		ImGui::End();
	}
	std::string script_registry_panel::get_menu_text() {
		return "Script registry";
	}
	void script_registry_panel::refresh() {
		this->m_scripts = thebanana::g_game->get_script_registry()->get_script_names();
	}
}