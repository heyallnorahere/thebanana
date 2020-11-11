#include <thebanana.h>
#include "property_editor_panel.h"
#include <imgui.h>
#include <backends/imgui_impl_win32.h>
#include <backends/imgui_impl_opengl3.h>
#include <misc/cpp/imgui_stdlib.h>
#include "scene_hierarchy_panel.h"
namespace bananatree {
	static void transform_menu(bool* open) {
		ImGui::Begin("Transform menu", open);
		ImGui::End();
	}
	property_editor_panel::property_editor_panel() {
		this->m_show_transform_menu = false;
		this->m_hierarchy = NULL;
	}
	void property_editor_panel::render() {
		if (this->m_show_transform_menu) transform_menu(&this->m_show_transform_menu);
		ImGui::Begin("Property Editor", &this->m_open);
		assert(this->m_hierarchy);
		thebanana::gameobject* object = this->m_hierarchy->get_selected_object();
		if (object) {
			ImGui::InputText("Gameobject Nickname", &object->get_nickname());
			char buf[256];
			_ui64toa(object->get_uuid(), buf, 10);
			ImGui::Text("Gameobject UUID: %s", buf);
			if (ImGui::Button("Toggle transform menu")) this->m_show_transform_menu = !this->m_show_transform_menu;
			for (size_t i = 0; i < object->get_number_components<thebanana::component>(); i++) {
				thebanana::component& c = object->get_component<thebanana::component>(i);
				const char* label = NULL;
				if (typeid(c).hash_code() == typeid(thebanana::tag_component).hash_code()) label = "Tag component";
				else if (typeid(c).hash_code() == typeid(thebanana::mesh_component).hash_code()) label = "Mesh component";
				else if (typeid(c).hash_code() == typeid(thebanana::animation_component).hash_code()) label = "Animation component";
				else if (typeid(c).hash_code() == typeid(thebanana::rigidbody).hash_code()) label = "Rigidbody";
				else if (typeid(c).hash_code() == typeid(thebanana::native_script_component).hash_code()) label = "Native script component";
				else if (typeid(c).hash_code() == typeid(thebanana::debug_component).hash_code()) label = "Debug component";
				assert(label);
				if (ImGui::CollapsingHeader(label)) {
					for (auto& p : c.get_properties()) {
						p->draw();
					}
				}
			}
		} else {
			ImGui::Text("There is no Gameobject selected. You can select one from the scene's hierarchy panel.");
		}
		ImGui::End();
	}
	std::string property_editor_panel::get_menu_text() {
		return "Property Editor";
	}
	void property_editor_panel::set_hierarchy(scene_hierarchy_panel* hierarchy) {
		this->m_hierarchy = hierarchy;
	}
}