#include <thebanana.h>
#include "project_editor_panel.h"
#include <imgui.h>
#include <backends/imgui_impl_win32.h>
#include <backends/imgui_impl_opengl3.h>
#include <misc/cpp/imgui_stdlib.h>
#include "../project.h"
#include "../util.h"
#include "../editor_layer.h"
namespace bananatree {
	void project_editor_panel::render() {
		ImGui::Begin("Project editor", &this->m_open);
		ImGui::InputText("Name", &this->m_current_name);
		ImGui::InputText("Main scene", &this->m_current_main_scene_path);
		ImGui::InputText("Project folder", &this->m_current_code_project_path);
		ImGui::SameLine();
		if (ImGui::Button("...")) {
			std::string path = util::open_dialog("Visual Studio Solution (*.sln)\0*.sln\0");
			if (!path.empty()) {
				size_t pos = path.find_last_of('\\');
				if (pos == std::string::npos) {
					this->m_current_code_project_path = ".";
				} else {
					this->m_current_code_project_path = path.substr(0, pos + 1);
				}
			}
		}
		ImGui::InputText("Output DLL name", &this->m_current_dll_name);
		if (ImGui::Button("Save")) {
			this->m_project->rename(this->m_current_name);
			this->m_project->set_main_scene_path(this->m_current_main_scene_path);
			bool same_path = (this->m_current_code_project_path == this->m_project->get_code_project_path());
			bool same_name = (this->m_current_dll_name == this->m_project->get_dll_name());
			this->m_project->set_code_project_path(this->m_current_code_project_path);
			this->m_project->set_dll_name(this->m_current_dll_name);
			if ((!same_path) && (!same_name)) {
				this->m_project->get_editor_layer()->compile_scripts();
			}
		}
		ImGui::End();
	}
	std::string project_editor_panel::get_menu_text() {
		return "Project editor";
	}
	void project_editor_panel::set_project(const std::shared_ptr<project>& p) {
		this->m_project = p;
		this->m_current_name = this->m_project->get_name();
		this->m_current_main_scene_path = this->m_project->get_main_scene_path();
		this->m_current_code_project_path = this->m_project->get_code_project_path();
	}
	void project_editor_panel::set_current_name(const std::string& name) {
		this->m_current_name = name;
	}
	void project_editor_panel::set_current_main_scene(const std::string& path) {
		this->m_current_main_scene_path = path;
	}
	void project_editor_panel::set_current_code_project(const std::string& path) {
		this->m_current_code_project_path = path;
	}
	void project_editor_panel::set_current_dll_name(const std::string& name) {
		this->m_current_dll_name = name;
	}
}