#include <thebanana.h>
#include "scene_hierarchy_panel.h"
#include <imgui.h>
#include <backends/imgui_impl_win32.h>
#include <backends/imgui_impl_opengl3.h>
#include <misc/cpp/imgui_stdlib.h>
namespace thebanana {
	namespace debug {
		extern gameobject* current_selected_gameobject;
	}
}
namespace bananatree {
	extern void create_static_mesh(bool* open, thebanana::gameobject* parent);
	void delete_object(thebanana::gameobject* child) {
		thebanana::gameobject* parent = child->get_parent();
		thebanana::scene* scene = child->get_scene();
		size_t index = std::string::npos;
		for (size_t i = 0; i < (parent ? parent->get_children_count() : scene->get_children_count()); i++) {
			thebanana::gameobject* obj = (parent ? parent->get_child(i) : scene->get_child(i));
			assert(obj);
			if (child == obj) {
				index = i;
				break;
			}
		}
		assert(index != std::string::npos);
		if (thebanana::debug::current_selected_gameobject == child)
			thebanana::debug::current_selected_gameobject = NULL;
		if (parent) {
			parent->remove_object(index);
		} else {
			scene->remove_object(index);
		}
	}
	bool scene_hierarchy_panel::gameobject_context_menu(thebanana::gameobject* object) {
		if (ImGui::BeginPopupContextItem()) {
			if (ImGui::BeginMenu("Add child")) {
				if (ImGui::MenuItem("Empty")) {
					thebanana::gameobject* object_ = object->add_object(new thebanana::basic_gameobject);
#ifndef _DEBUG
					object_->add_component<thebanana::debug_component>();
#endif
				}
				if (ImGui::MenuItem("Static mesh")) {
					this->m_open_static_mesh_menu = true;
					this->m_current_static_mesh_parent = object;
				}
				ImGui::EndMenu();
			}
			if (ImGui::MenuItem("Remove object")) {
				delete_object(object);
			}
			ImGui::EndPopup();
			return true;
		}
		return false;
	}
	// the second parameter is for use later
	void set_gameobject(thebanana::gameobject* object, thebanana::gameobject** object_ptr) {
		if (ImGui::IsItemClicked(ImGuiMouseButton_Left)) {
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
		this->m_open_static_mesh_menu = false;
		this->m_current_static_mesh_parent = NULL;
	}
	void scene_hierarchy_panel::render() {
		id = 0xff;
		ImGui::Begin("Scene Hierarchy", &this->m_open);
		if (ImGui::TreeNodeEx("Root", ImGuiTreeNodeFlags_Leaf)) {
			set_gameobject(NULL, &thebanana::debug::current_selected_gameobject);
			for (size_t i = 0; i < thebanana::g_game->get_scene()->get_children_count(); i++) {
				thebanana::gameobject* object = thebanana::g_game->get_scene()->get_child(i);
				this->tree_helper(object);
			}
			ImGui::TreePop();
		}
		ImGui::End();
		if (this->m_open_static_mesh_menu) create_static_mesh(&this->m_open_static_mesh_menu, this->m_current_static_mesh_parent);
	}
	std::string scene_hierarchy_panel::get_menu_text() {
		return "Scene Hierarchy";
	}
	thebanana::gameobject* scene_hierarchy_panel::get_selected_object() {
		return thebanana::debug::current_selected_gameobject;
	}
	void scene_hierarchy_panel::set_selected_object(thebanana::gameobject* object) {
		thebanana::debug::current_selected_gameobject = object;
	}
	void make_dragdrop_source(thebanana::gameobject* object) {
		ImGui::SameLine();
		ImGui::ColorButton("Drag/Drop Source", ImVec4(0.5f, 0.5f, 0.5f, 1.f), ImGuiColorEditFlags_None, ImVec2(15.f, 15.f));
		if (ImGui::BeginDragDropSource()) {
			unsigned long long uuid = object->get_uuid();
			ImGui::SetDragDropPayload("GAMEOBJECT_PAYLOAD", &uuid, sizeof(unsigned long long));
			ImGui::Text(object->get_nickname().c_str());
			ImGui::EndDragDropSource();
		}
	}
	constexpr ImGuiTreeNodeFlags open_flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick;
	void scene_hierarchy_panel::tree_helper(thebanana::gameobject* object) {
		if (object->get_children_count() > 0) {
			this->tree(object);
		} else {
			std::string id_str = make_id();
			if (ImGui::TreeNodeEx(id_str.c_str(), ImGuiTreeNodeFlags_Leaf, "%s, no children", object->get_nickname().c_str())) {
				this->gameobject_context_menu(object);
				make_dragdrop_source(object);
				set_gameobject(object, &thebanana::debug::current_selected_gameobject);
				ImGui::TreePop();
			} else {
				this->gameobject_context_menu(object);
				make_dragdrop_source(object);
			}
		}
	}
	void scene_hierarchy_panel::tree(thebanana::gameobject* object) {
		std::string id_str = make_id();
		if (ImGui::TreeNodeEx(id_str.c_str(), open_flags, "%s, children: %d", object->get_nickname().c_str(), object->get_children_count())) {
			this->gameobject_context_menu(object);
			make_dragdrop_source(object);
			set_gameobject(object, &thebanana::debug::current_selected_gameobject);
			for (size_t i = 0; i < object->get_children_count(); i++) {
				thebanana::gameobject* child = object->get_child(i);
				this->tree_helper(child);
			}
			ImGui::TreePop();
		}
	}
}