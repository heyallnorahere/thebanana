#include <thebanana.h>
#include "scene_hierarchy_panel.h"
#include <imgui.h>
#include <backends/imgui_impl_win32.h>
#include <backends/imgui_impl_opengl3.h>
#include <misc/cpp/imgui_stdlib.h>
namespace bananatree {
	// the second parameter is for use later
	void set_gameobject(thebanana::gameobject* object, thebanana::gameobject** object_ptr) {
		if (ImGui::IsItemClicked()) {
			*object_ptr = object;
		}
	}
	static int id;
	static std::string make_id() {
		char id_str[256];
		sprintf(id_str, "node%d", id++);
		return id_str;
	}
	scene_hierarchy_panel::scene_hierarchy_panel() {
		this->m_selected_object = NULL;
	}
	void scene_hierarchy_panel::render() {
		id = 0xff;
		ImGui::Begin("Scene Hierarchy", &this->m_open);
		if (ImGui::TreeNodeEx("Root", ImGuiTreeNodeFlags_Leaf)) {
			set_gameobject(NULL, &this->m_selected_object);
			for (size_t i = 0; i < thebanana::g_game->get_scene()->get_children_count(); i++) {
				thebanana::gameobject* object = thebanana::g_game->get_scene()->get_child(i);
				this->tree_helper(object);
			}
			ImGui::TreePop();
		}
		ImGui::End();
	}
	std::string scene_hierarchy_panel::get_menu_text() {
		return "Scene Hierarchy";
	}
	thebanana::gameobject* scene_hierarchy_panel::get_selected_object() {
		return this->m_selected_object;
	}
	void scene_hierarchy_panel::set_selected_object(thebanana::gameobject* object) {
		this->m_selected_object = object;
	}
	void make_dragdrop_source(thebanana::gameobject* object, const std::string& id) {
		ImGui::SameLine();
		std::string source_id = id + "dnds";
		ImGui::PushID(source_id.c_str());
		ImGui::ColorButton("Drag/Drop Source", ImVec4(0.5f, 0.5f, 0.5f, 1.f), ImGuiColorEditFlags_None, ImVec2(15.f, 15.f));
		if (ImGui::BeginDragDropSource()) {
			unsigned long long uuid = object->get_uuid();
			ImGui::SetDragDropPayload("GAMEOBJECT_PAYLOAD", &uuid, sizeof(unsigned long long));
			ImGui::Text(object->get_nickname().c_str());
			ImGui::EndDragDropSource();
		}
		ImGui::PopID();
	}
	constexpr ImGuiTreeNodeFlags open_flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick;
	void scene_hierarchy_panel::tree_helper(thebanana::gameobject* object) {
		if (object->get_children_count() > 0) {
			this->tree(object);
		} else {
			std::string id_str = make_id();
			if (ImGui::TreeNodeEx(id_str.c_str(), ImGuiTreeNodeFlags_Leaf, "%s, no children", object->get_nickname().c_str())) {
				set_gameobject(object, &this->m_selected_object);
				ImGui::TreePop();
			}
			make_dragdrop_source(object, id_str);
		}
	}
	void scene_hierarchy_panel::tree(thebanana::gameobject* object) {
		std::string id_str = make_id();
		if (ImGui::TreeNodeEx(id_str.c_str(), open_flags, "%s, children: %d", object->get_nickname().c_str(), object->get_children_count())) {
			set_gameobject(object, &this->m_selected_object);
			for (size_t i = 0; i < object->get_children_count(); i++) {
				thebanana::gameobject* child = object->get_child(i);
				this->tree_helper(child);
			}
			ImGui::TreePop();
		}
		make_dragdrop_source(object, id_str);
	}
}