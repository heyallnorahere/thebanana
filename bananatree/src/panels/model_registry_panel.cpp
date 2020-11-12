#include <thebanana.h>
#include "model_registry_panel.h"
#include <imgui.h>
#include <backends/imgui_impl_win32.h>
#include <backends/imgui_impl_opengl3.h>
#include <misc/cpp/imgui_stdlib.h>
namespace bananatree {
	static void import_menu(bool* open, model_registry_panel* panel) {
		ImGui::Begin("Import model", open, ImGuiWindowFlags_NoDocking);
		static std::string path = "";
		static std::string name = "";
		ImGui::InputText("Path", &path);
		ImGui::InputText("Name", &name);
		if (ImGui::Button("Cancel")) {
			path = "";
			name = "";
			*open = false;
		}
		ImGui::SameLine();
		if (ImGui::Button("Import")) {
			panel->import(path, name);
			path = "";
			name = "";
			*open = false;
		}
		ImGui::End();
	}
	model_registry_panel::model_registry_panel() {
		this->m_import_menu_open = false;
	}
	void model_registry_panel::render() {
		if (this->m_import_menu_open) import_menu(&this->m_import_menu_open, this);
		ImGui::Begin("Model registry", &this->m_open, ImGuiWindowFlags_MenuBar);
		if (ImGui::BeginMenuBar()) {
			if (ImGui::BeginMenu("Model")) {
				if (ImGui::MenuItem("Import...", "Ctrl+Shift+I")) {
					this->m_import_menu_open = true;
				}
				ImGui::EndMenu();
			}
			ImGui::EndMenuBar();
		}
		if (ImGui::CollapsingHeader("Loaded models")) {
			for (auto m : thebanana::g_game->get_model_registry()->get_loaded_model_names()) {
				ImGui::Text(m.c_str());
			}
		}
		ImGui::End();
	}
	std::string model_registry_panel::get_menu_text() {
		return "Model registry";
	}
	void model_registry_panel::import(const std::string& path, const std::string& name) {
		thebanana::g_game->get_model_registry()->load({ { name, path, NULL, thebanana::transform() } });
	}
}