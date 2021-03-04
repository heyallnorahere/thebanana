#include <thebanana.h>
#include "model_registry_panel.h"
#include <imgui.h>
#include <backends/imgui_impl_win32.h>
#include <backends/imgui_impl_opengl3.h>
#include <misc/cpp/imgui_stdlib.h>
#include "../util.h"
namespace bananatree {
	extern void vec3_control(const std::string& label, glm::vec3& vector, float reset_value = 0.f, float speed = 0.1f, float column_width = 100.f);
	static void import_menu(bool* open, model_registry_panel* panel) {
		ImGui::Begin("Import model", open, ImGuiWindowFlags_NoDocking);
		static std::string path = "";
		static std::string name = "";
		static thebanana::transform transform;
		ImGui::InputText("Path", &path);
		ImGui::SameLine();
		if (ImGui::Button("...")) {
			std::string path_ = util::open_dialog("OBJ file (*.obj)\0*.obj\0FBX file (*.fbx)\0*.fbx\0");
			if (!path_.empty()) path = path_;
		}
		ImGui::InputText("Name", &name);
		glm::vec3 translation = transform.get_translation();
		glm::vec3 rotation = transform.get_rotation();
		rotation.x = glm::degrees(rotation.x);
		rotation.y = glm::degrees(rotation.y);
		rotation.z = glm::degrees(rotation.z);
		glm::vec3 scale = transform.get_scale();
		vec3_control("Translation", translation);
		vec3_control("Rotation", rotation);
		vec3_control("Scale", scale, 1.f);
		transform.set_translation(translation);
		rotation.x = glm::radians(rotation.x);
		rotation.y = glm::radians(rotation.y);
		rotation.z = glm::radians(rotation.z);
		transform.set_rotation(rotation);
		if (scale.x < 0.001f) scale.x = 0.001f;
		if (scale.y < 0.001f) scale.y = 0.001f;
		if (scale.z < 0.001f) scale.z = 0.001f;
		transform.set_scale(scale);
		if (ImGui::Button("Cancel")) {
			path = "";
			name = "";
			transform = thebanana::transform();
			*open = false;
		}
		ImGui::SameLine();
		if (ImGui::Button("Import")) {
			panel->import(path, name, transform);
			path = "";
			name = "";
			transform = thebanana::transform();
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
	void model_registry_panel::import(const std::string& path, const std::string& name, const thebanana::transform& transform) {
		this->m_project->register_model({ path, name, transform });
		thebanana::g_game->get_model_registry()->load({ { name, path, transform } });
	}
	void model_registry_panel::import(const project::model_descriptor& md) {
		thebanana::g_game->get_model_registry()->load({ { md.name, md.path, md.model_transform } });
	}
	void model_registry_panel::set_project(const std::shared_ptr<project>& p) {
		this->m_project = p;
	}
}