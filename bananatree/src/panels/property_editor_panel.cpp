#include <thebanana.h>
#include "property_editor_panel.h"
#include <imgui.h>
#include <imgui_internal.h>
#include <backends/imgui_impl_win32.h>
#include <backends/imgui_impl_opengl3.h>
#include <misc/cpp/imgui_stdlib.h>
#include "scene_hierarchy_panel.h"
namespace bananatree {
	static void transform_menu(thebanana::gameobject* object, bool* open) {
		if (!object) return;
		ImGui::Begin("Transform menu", open, ImGuiWindowFlags_NoDocking);
		if (ImGui::CollapsingHeader("Move")) {
			static glm::vec3 m = glm::vec3(0.f);
			ImGui::DragFloat3("Motion values", &m.x);
			if (ImGui::Button("Apply motion")) {
				object->get_transform().move(m);
				m = glm::vec3(0.f);
			}
		}
		if (ImGui::CollapsingHeader("Translate")) {
			static glm::vec3 t = glm::vec3(0.f);
			ImGui::DragFloat3("Translation values", &t.x);
			if (ImGui::Button("Apply translation")) {
				object->get_transform().translate(t);
				t = glm::vec3(0.f);
			}
		}
		if (ImGui::CollapsingHeader("Rotate")) {
			static glm::vec3 r = glm::vec3(0.f);
			ImGui::DragFloat3("Rotation values", &r.x);
			if (ImGui::Button("Apply rotation")) {
				if (fabs(r.x) > 0.001) object->get_transform().rotate(r.x, glm::vec3(1.f, 0.f, 0.f));
				if (fabs(r.y) > 0.001) object->get_transform().rotate(r.y, glm::vec3(0.f, 1.f, 0.f));
				if (fabs(r.z) > 0.001) object->get_transform().rotate(r.z, glm::vec3(0.f, 0.f, 1.f));
				r = glm::vec3(0.f);
			}
		}
		if (ImGui::CollapsingHeader("Scale")) {
			static glm::vec3 s = glm::vec3(1.f);
			ImGui::InputFloat3("Scaling values", &s.x);
			if (ImGui::Button("Apply scaling")) {
				object->get_transform().scale(s);
				s = glm::vec3(1.f);
			}
		}
		ImGui::End();
	}
	property_editor_panel::property_editor_panel() {
		this->m_show_transform_menu = false;
		this->m_component_index = 0;
		this->m_hierarchy = NULL;
	}
	static void script_dragdrop_target(thebanana::native_script_component& component, thebanana::component::property_base* prop) {
		auto property = (thebanana::component::property<thebanana::component::property_base::read_only_text>*)prop;
		bool has_script = (property->get_value()->get_text() != "none");
		std::string id_string = std::to_string(component.get_uuid()) + " dragdrop source";
		ImGui::PushID(id_string.c_str());
		std::string text = (has_script ? property->get_value()->get_text() : "None");
		ImGui::InputText(property->get_name().c_str(), &text, ImGuiInputTextFlags_ReadOnly);
		if (ImGui::BeginDragDropTarget()) {
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("SCRIPT_PAYLOAD")) {
				if (component.get_script<thebanana::script>()) {
					auto& properties = component.get_properties();
					std::vector<std::string> names;
					for (size_t i = 1; i < properties.size(); i++) {
						auto it = properties.begin();
						std::advance(it, i);
						auto& p = *it;
						names.push_back(p->get_name());
					}
					for (const auto& name : names) {
						component.remove_property(name);
					}
				}
				std::string script_name = std::string((char*)payload->Data, payload->DataSize);
				thebanana::script* script = thebanana::g_game->get_script_registry()->create_script(script_name, component.get_parent(), &component);
				component.bind(script);
				component.set_property("Script", script_name);
				script->initialize();
			}
			ImGui::EndDragDropTarget();
		}
		ImGui::PopID();
	}
	void property_editor_panel::render() {
		thebanana::gameobject* object = this->m_hierarchy->get_selected_object();
		if (this->m_show_transform_menu) transform_menu(object, &this->m_show_transform_menu);
		ImGui::Begin("Property Editor", &this->m_open, ImGuiWindowFlags_MenuBar);
		assert(this->m_hierarchy);
		if (ImGui::BeginMenuBar()) {
			if (ImGui::BeginMenu("Component")) {
				if (ImGui::MenuItem("Tag component")) {
					if (object) {
						object->add_component<thebanana::tag_component>();
					} else {
						thebanana::g_game->debug_print("could not add tag component; no object selected");
					}
				}
				if (ImGui::MenuItem("Mesh component")) {
					if (object) {
						object->add_component<thebanana::mesh_component>();
					} else {
						thebanana::g_game->debug_print("could not add mesh component; no object selected");
					}
				}
				if (ImGui::MenuItem("Animation component")) {
					if (object) {
						object->add_component<thebanana::animation_component>();
					} else {
						thebanana::g_game->debug_print("could not add animation component; no object selected");
					}
				}
				if (ImGui::MenuItem("Rigidbody")) {
					if (object) {
						object->add_component<thebanana::rigidbody>();
					} else {
						thebanana::g_game->debug_print("could not add rigidbody; no object selected");
					}
				}
				if (ImGui::MenuItem("Camera component")) {
					if (object) {
						object->add_component<thebanana::camera_component>();
					} else {
						thebanana::g_game->debug_print("could not add camera component; no object selected");
					}
				}
				if (ImGui::MenuItem("Particlesystem component")) {
					if (object) {
						object->add_component<thebanana::particlesystem::particlesystem_component>();
					}
					else {
						thebanana::g_game->debug_print("could not add particlesystem component; no object selected");
					}
				}
				if (ImGui::MenuItem("Native script component")) {
					if (object) {
						object->add_component<thebanana::native_script_component>();
					} else {
						thebanana::g_game->debug_print("could not add native script component; no object selected");
					}
				}
				ImGui::EndMenu();
			}
			ImGui::EndMenuBar();
		}
		if (object) {
			ImGui::InputText("Gameobject Nickname", &object->get_nickname());
			char buf[256];
			_ui64toa(object->get_uuid(), buf, 10);
			ImGui::Text("Gameobject UUID: %s", buf);
			if (ImGui::Button("Toggle transform menu")) this->m_show_transform_menu = !this->m_show_transform_menu;
			ImGui::Separator();
			ImGui::Text("Components:");
			for (size_t i = 0; i < object->get_number_components<thebanana::component>(); i++) {
				thebanana::component& c = object->get_component<thebanana::component>(i);
				const char* label = NULL;
				if (typeid(c).hash_code() == typeid(thebanana::tag_component).hash_code()) label = "Tag component";
				else if (typeid(c).hash_code() == typeid(thebanana::mesh_component).hash_code()) label = "Mesh component";
				else if (typeid(c).hash_code() == typeid(thebanana::animation_component).hash_code()) label = "Animation component";
				else if (typeid(c).hash_code() == typeid(thebanana::rigidbody).hash_code()) label = "Rigidbody";
				else if (typeid(c).hash_code() == typeid(thebanana::camera_component).hash_code()) label = "Camera component";
				else if (typeid(c).hash_code() == typeid(thebanana::particlesystem::particlesystem_component).hash_code()) label = "Particlesystem component";
				else if (typeid(c).hash_code() == typeid(thebanana::native_script_component).hash_code()) label = "Native script component";
				else if (typeid(c).hash_code() == typeid(thebanana::debug_component).hash_code()) label = "Debug component";
				assert(label);
				char buf[256];
				_ui64toa(c.get_uuid(), buf, 10);
				std::string label_text = label + std::string(", UUID: ") + buf;
				if (ImGui::CollapsingHeader(label_text.c_str())) {
					bool should_remove = false;
					ImVec2 content_region_available = ImGui::GetContentRegionAvail();
					float line_height = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
					float indent = content_region_available.x - line_height;
					ImGui::Indent(indent);
					if (ImGui::Button("+", ImVec2(line_height, line_height))) {
						ImGui::OpenPopup("component_settings");
					}
					ImGui::Unindent(indent);
					if (ImGui::BeginPopup("component_settings")) {
						if (ImGui::MenuItem("Remove component")) {
							should_remove = true;
						}
						ImGui::EndPopup();
					}
					auto& properties = c.get_properties();
					for (size_t i = 0; i < properties.size(); i++) {
						auto it = properties.begin();
						std::advance(it, i);
						auto& p = *it;
						if (typeid(*p).hash_code() == typeid(thebanana::component::property<thebanana::gameobject*>).hash_code()) {
							thebanana::component::property<thebanana::gameobject*>* prop = (thebanana::component::property<thebanana::gameobject*>*)p.get();
							char buf[256];
							_ui64toa(c.get_uuid(), buf, 10);
							std::string uuidstring = buf;
							_ui64toa(i, buf, 10);
							char id[256];
							sprintf(id, "%s, %s", uuidstring.c_str(), buf);
							ImGui::PushID(id);
							std::string text = (*prop->get_value()) ? (*prop->get_value())->get_nickname() : "None";
							ImGui::InputText(prop->get_name().c_str(), &text, ImGuiInputTextFlags_ReadOnly);
							if (ImGui::BeginDragDropTarget()) {
								if (const ImGuiPayload * payload = ImGui::AcceptDragDropPayload("GAMEOBJECT_PAYLOAD")) {
									assert(payload->DataSize == sizeof(unsigned long long));
									unsigned long long uuid = *(unsigned long long*)payload->Data;
									*prop->get_value() = thebanana::g_game->get_scene()->find(uuid);
								}
								ImGui::EndDragDropTarget();
							}
							ImGui::PopID();
							if (ImGui::Button("Clear property")) {
								*prop->get_value() = NULL;
							}
						} else if (p->get_name() == "Script") {
							script_dragdrop_target((thebanana::native_script_component&)c, p.get());
						} else {
							p->draw();
						}
					}
					if (should_remove) {
						object->remove_component<thebanana::component>(i);
						i--;
					}
				}
			}
		} else {
			ImGui::Text("There is no Gameobject selected.\nYou can select one from the scene's hierarchy panel.");
			if (this->m_show_transform_menu) this->m_show_transform_menu = false;
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