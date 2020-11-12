#include <thebanana.h>
#include "model_registry_panel.h"
#include <imgui.h>
#include <backends/imgui_impl_win32.h>
#include <backends/imgui_impl_opengl3.h>
#include <misc/cpp/imgui_stdlib.h>
#include "../util.h"
namespace bananatree {
	struct frs {
		bool enabled;
		std::string find, replace;
	};
	static frs current;
	static void import_menu(bool* open, model_registry_panel* panel) {
		ImGui::Begin("Import model", open, ImGuiWindowFlags_NoDocking);
		static std::string path = "";
		static std::string name = "";
		static bool replace = false;
		static std::string find = "";
		static std::string replace_ = "";
		ImGui::InputText("Path", &path);
		ImGui::SameLine();
		if (ImGui::Button("...")) {
			std::string path_ = open_dialog("OBJ file (*.obj)\0*.obj\0FBX file (*.fbx)\0*.fbx\0");
			if (!path_.empty()) path = path_;
		}
		ImGui::InputText("Name", &name);
		ImGui::Checkbox("Replace texture paths", &replace);
		if (replace) {
			ImGui::InputText("Find", &find);
			ImGui::InputText("Replace", &replace_);
		}
		if (ImGui::Button("Cancel")) {
			path = "";
			name = "";
			replace = false;
			find = "";
			replace_ = "";
			*open = false;
		}
		ImGui::SameLine();
		if (ImGui::Button("Import")) {
			current = { replace, find, replace_ };
			panel->import(path, name);
			path = "";
			name = "";
			replace = false;
			find = "";
			replace_ = "";
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
	static std::string find_replace_proc(const std::string& path, void*) {
		return thebanana::model_registry::path_helper(path, current.find, current.replace);
	}
	void model_registry_panel::import(const std::string& path, const std::string& name) {
		this->m_project->register_model({ path, name, current.enabled, current.find, current.replace });
		thebanana::g_game->get_model_registry()->load({ { name, path, (current.enabled ? find_replace_proc : NULL), thebanana::transform() } });
	}
	void model_registry_panel::import(const project::model_descriptor& md) {
		current.enabled = md.should_replace;
		if (current.enabled) {
			current.find = md.find;
			current.replace = md.replace;
		}
		thebanana::g_game->get_model_registry()->load({ { md.name, md.path, (current.enabled ? find_replace_proc : NULL), thebanana::transform() } });
	}
	void model_registry_panel::set_project(const std::shared_ptr<project>& p) {
		this->m_project = p;
	}
}