#include <thebanana.h>
#include "material_editor_panel.h"
#include <imgui.h>
#include <backends/imgui_impl_win32.h>
#include <backends/imgui_impl_opengl3.h>
#include <misc/cpp/imgui_stdlib.h>
#include "../util.h"
namespace bananatree {
	void material_editor_panel::refresh() {
		this->m_index = 0;
		this->m_materials.clear();
		for (size_t i = 0; i < thebanana::g_game->get_material_registry()->get_count(); i++) {
			this->m_materials.push_back(thebanana::g_game->get_material_registry()->get(i));
		}
		this->m_descriptors.clear();
		auto& materials = this->m_project->get_materials();
		for (auto& m : materials) {
			this->m_descriptors.push_back((project::material_descriptor*)&m);
		}
	}
	void material_editor_panel::render() {
		std::vector<std::string> strings;
		std::vector<const char*> cstrings;
		strings.push_back("None");
		for (thebanana::material* mat : this->m_materials) {
			strings.push_back(mat->get_friendly_name());
		}
		for (const auto& str : strings) {
			cstrings.push_back(str.c_str());
		}
		ImGui::Begin("Material editor", &this->m_open);
		if (ImGui::Button("Refresh")) {
			this->refresh();
		}
		ImGui::Combo("##Material", &this->m_index, cstrings.data(), cstrings.size());
		ImGui::SameLine();
		if (ImGui::Button("New")) {
			unsigned long long uuid = thebanana::g_game->get_material_registry()->new_material();
			this->m_project->add_material();
			this->refresh();
			for (size_t i = 0; i < thebanana::g_game->get_material_registry()->get_count(); i++) {
				thebanana::material* mat = thebanana::g_game->get_material_registry()->get(i);
				if (mat->get_uuid() == uuid) {
					this->m_index = (int)i + 1;
				}
			}
			thebanana::material* mat = thebanana::g_game->get_material_registry()->get((size_t)this->m_index - 1);
			this->m_descriptors[(size_t)this->m_index - 1].copy.friendly_name = mat->get_friendly_name();
			this->m_descriptors[(size_t)this->m_index - 1].copy.diffuse = mat->get_diffuse();
			this->m_descriptors[(size_t)this->m_index - 1].copy.specular = mat->get_specular();
			this->m_descriptors[(size_t)this->m_index - 1].copy.ambient = mat->get_ambient();
			this->m_descriptors[(size_t)this->m_index - 1].copy.shininess = mat->get_shininess();
			this->m_descriptors[(size_t)this->m_index - 1].copy.uuid = mat->get_uuid();
			this->m_descriptors[(size_t)this->m_index - 1].commit();
		}
		size_t index = (size_t)this->m_index;
		if (index > 0) {
			index--;
			thebanana::material* mat = thebanana::g_game->get_material_registry()->get(index);
			ImGui::PushID("dragdropsource");
			ImGui::ColorButton("Drag/Drop Source", ImVec4(0.5f, 0.5f, 0.5f, 1.f), ImGuiColorEditFlags_None, ImVec2(ImGui::GetContentRegionAvail().x, 0.f));
			if (ImGui::BeginDragDropSource()) {
				unsigned long long uuid = mat->get_uuid();
				ImGui::SetDragDropPayload("MATERIAL_PAYLOAD", &uuid, sizeof(unsigned long long));
				ImGui::Text(mat->get_friendly_name().c_str());
				ImGui::EndDragDropSource();
			}
			ImGui::PopID();
			ImGui::InputText("Name", &this->m_descriptors[index].copy.friendly_name);
			if (ImGui::CollapsingHeader("Texture")) {
				ImGui::PushID("texture");
				ImGui::Image((ImTextureID)mat->get_texture()->get_id(), ImVec2(100.f, 100.f), ImVec2(0.f, 1.f), ImVec2(1.f, 0.f));
				ImGui::SameLine();
				if (ImGui::Button("...")) {
					std::string path = util::open_dialog("PNG Image (*.png)\0*.png\0JPEG Image (*.jpeg,*.jpg)\0*.jpeg,*.jpg\0");
					if (!path.empty()) {
						this->m_material_texture_ptr = mat->get_texture();
						this->m_descriptors[index].copy.texture_path = path;
					}
				}
				ImGui::SameLine();
				if (ImGui::Button("Clear")) {
					this->m_material_texture_ptr = mat->get_texture();
					unsigned char texture_color[3];
					memset(texture_color, 0xff, 3);
					mat->set_texture(texture_color, 1, 1, 3);
					this->m_descriptors[index].copy.texture_path = mat->get_texture_path();
				}
				ImGui::PopID();
			}
			if (ImGui::CollapsingHeader("Normal map")) {
				ImGui::PushID("normalmap");
				ImGui::Image((ImTextureID)mat->get_normal_map()->get_id(), ImVec2(100.f, 100.f), ImVec2(0.f, 1.f), ImVec2(1.f, 0.f));
				ImGui::SameLine();
				if (ImGui::Button("...")) {
					std::string path = util::open_dialog("PNG Image (*.png)\0*.png\0JPEG Image (*.jpeg,*.jpg)\0*.jpeg,*.jpg\0");
					if (!path.empty()) {
						this->m_material_normal_map_ptr = mat->get_normal_map();
						this->m_descriptors[index].copy.normal_map_path = path;
					}
				}
				ImGui::SameLine();
				if (ImGui::Button("Clear")) {
					this->m_material_normal_map_ptr = mat->get_normal_map();
					unsigned char normal_map_color[3];
					memset(normal_map_color, 0xff, 3);
					mat->set_normal_map(normal_map_color, 1, 1, 3);
					this->m_descriptors[index].copy.normal_map_path = mat->get_normal_map_path();
				}
				ImGui::PopID();
			}
			if (ImGui::CollapsingHeader("Settings")) {
				ImGui::ColorEdit3("Diffuse color", &this->m_descriptors[index].copy.diffuse.x);
				ImGui::ColorEdit3("Specular color", &this->m_descriptors[index].copy.specular.x);
				ImGui::ColorEdit3("Ambient color", &this->m_descriptors[index].copy.ambient.x);
				ImGui::DragFloat("Shininess", &this->m_descriptors[index].copy.shininess, 0.001f, 0.f, 1.f);
			}
		} else {
			ImGui::Text("No material selected");
		}
		for (size_t i = 0; i < thebanana::g_game->get_material_registry()->get_count(); i++) {
			thebanana::material* mat = thebanana::g_game->get_material_registry()->get(i);
			if (this->m_descriptors[i].copy.texture_path != mat->get_texture_path())
				mat->set_texture(this->m_descriptors[i].copy.texture_path);
			if (this->m_descriptors[i].copy.normal_map_path != mat->get_normal_map_path())
				mat->set_normal_map(this->m_descriptors[i].copy.normal_map_path);
			mat->set_friendly_name(this->m_descriptors[i].copy.friendly_name);
			mat->set_diffuse(this->m_descriptors[i].copy.diffuse);
			mat->set_specular(this->m_descriptors[i].copy.specular);
			mat->set_ambient(this->m_descriptors[i].copy.ambient);
			mat->set_shininess(this->m_descriptors[i].copy.shininess);
			this->m_descriptors[i].commit();
		}
		ImGui::End();
	}
	std::string material_editor_panel::get_menu_text() {
		return std::string();
	}
	void material_editor_panel::set_project(const std::shared_ptr<project>& p) {
		this->m_project = p;
		this->refresh();
	}
	void material_editor_panel::add_material(const project::material_descriptor& md) {
		thebanana::material_registry* registry = thebanana::g_game->get_material_registry();
		thebanana::material* mat = registry->find(registry->new_material());
		mat->set_friendly_name(md.friendly_name);
		mat->set_texture(md.texture_path);
		mat->set_normal_map(md.normal_map_path);
		mat->set_diffuse(md.diffuse);
		mat->set_specular(md.specular);
		mat->set_ambient(md.ambient);
		mat->set_shininess(md.shininess);
		mat->set_uuid(md.uuid);
	}
}