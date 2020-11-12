#include <thebanana.h>
#include "project_editor_panel.h"
#include <imgui.h>
#include <backends/imgui_impl_win32.h>
#include <backends/imgui_impl_opengl3.h>
#include <misc/cpp/imgui_stdlib.h>
#include "../project.h"
namespace bananatree {
	void project_editor_panel::render() {
		ImGui::Begin("Project editor", &this->m_open);
		ImGui::InputText("Name", &this->m_current_name);
		ImGui::SameLine();
		if (ImGui::Button("Save")) {
			this->m_project->rename(this->m_current_name);
		}
		ImGui::End();
	}
	std::string project_editor_panel::get_menu_text() {
		return "Project editor";
	}
	void project_editor_panel::set_project(const std::shared_ptr<project>& p) {
		this->m_project = p;
		this->m_current_name = this->m_project->get_name();
	}
}