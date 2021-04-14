#include <thebanana.h>
#include "property_editor_panel.h"
#include <imgui.h>
#include <imgui_internal.h>
#include <backends/imgui_impl_win32.h>
#include <backends/imgui_impl_opengl3.h>
#include <misc/cpp/imgui_stdlib.h>
#include "scene_hierarchy_panel.h"
#include "texture_viewer_panel.h"
namespace bananatree {
	property_editor_panel::property_editor_panel() {
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
	void vec3_control(const std::string& label, glm::vec3& vector, float reset_value = 0.f, float speed = 0.1f, float column_width = 100.f) {
		ImGuiIO& io = ImGui::GetIO();
		auto bold_font = io.Fonts->Fonts[0];
		ImGui::PushID(label.c_str());
		ImGui::Columns(2);
		ImGui::SetColumnWidth(0, column_width);
		ImGui::Text(label.c_str());
		ImGui::NextColumn();
		ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0.f, 0.f));
		float line_height = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.f;
		ImVec2 button_size = ImVec2(line_height + 3.f, line_height);
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.8f, 0.1f, 0.15f, 1.0f));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.9f, 0.2f, 0.2f, 1.0f));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.8f, 0.1f, 0.15f, 1.0f));
		ImGui::PushFont(bold_font);
		if (ImGui::Button("X", button_size)) {
			vector.x = reset_value;
		}
		ImGui::PopFont();
		ImGui::PopStyleColor(3);
		ImGui::SameLine();
		ImGui::DragFloat("##X", &vector.x, speed);
		ImGui::PopItemWidth();
		ImGui::SameLine();
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.2f, 0.7f, 0.2f, 1.0f));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.3f, 0.8f, 0.3f, 1.0f));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.2f, 0.7f, 0.2f, 1.0f));
		ImGui::PushFont(bold_font);
		if (ImGui::Button("Y", button_size)) {
			vector.y = reset_value;
		}
		ImGui::PopFont();
		ImGui::PopStyleColor(3);
		ImGui::SameLine();
		ImGui::DragFloat("##Y", &vector.y, speed);
		ImGui::PopItemWidth();
		ImGui::SameLine();
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.1f, 0.25f, 0.8f, 1.0f));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.2f, 0.35f, 0.9f, 1.0f));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.1f, 0.25f, 0.8f, 1.0f));
		ImGui::PushFont(bold_font);
		if (ImGui::Button("Z", button_size)) {
			vector.z = reset_value;
		}
		ImGui::PopFont();
		ImGui::PopStyleColor(3);
		ImGui::SameLine();
		ImGui::DragFloat("##Z", &vector.z, speed);
		ImGui::PopItemWidth();
		ImGui::PopStyleVar();
		ImGui::Columns(1);
		ImGui::PopID();
	}
	static void collider_control(thebanana::rigidbody& rb) {
		ImGui::Text("Collider settings");
		std::vector<const char*> strings;
		strings.push_back("None");
		strings.push_back("Sphere");
		strings.push_back("Rectangular prism");
		int current, index;
		if (rb.get_collider()) {
			size_t hash_code = typeid(*rb.get_collider()).hash_code();
			if (hash_code == typeid(thebanana::mlfarrel_model).hash_code()) {
				current = 1;
			} else {
				assert(false);
			}
		} else {
			current = 0;
		}
		index = current;
		ImGui::Combo("Type", &index, strings.data(), strings.size());
		if (index != current) {
			switch (index) {
			case 0:
				rb.set_collider_type<thebanana::collider>();
				break;
			case 1:
				rb.set_collider_type<thebanana::mlfarrel_model>();
				break;
			case 2:
				rb.set_collider_type<thebanana::rectangular_prism_collider>();
				break;
			}
			current = index;
		}
		switch (current) {
		case 1:
		{
			thebanana::mlfarrel_model* c = (thebanana::mlfarrel_model*)rb.get_collider();
			glm::vec3 offset = c->get_origin_offset();
			ImGui::DragFloat3("Origin offset", &offset.x);
			c->set_origin_offset(offset);
			float radius = c->get_radius();
			ImGui::DragFloat("Radius", &radius);
			c->set_radius(radius);
		}
			break;
		case 2:
		{
			thebanana::rectangular_prism_collider* c = (thebanana::rectangular_prism_collider*)rb.get_collider();
			glm::vec3 offset = c->get_origin_offset();
			ImGui::DragFloat3("Origin offset", &offset.x);
			c->set_origin_offset(offset);
			glm::vec3 dimensions = c->get_dimensions();
			ImGui::DragFloat3("Dimensions", &dimensions.x);
			c->set_dimensions(dimensions);
		}
			break;
		}
		bool check_for_collisions = rb.is_checking_for_collisions();
		ImGui::Checkbox("Check for collisions", &check_for_collisions);
		rb.set_check_for_collisions(check_for_collisions);
		ImGui::Separator();
	}
	void property_editor_panel::render() {
		thebanana::gameobject* object = this->m_hierarchy->get_selected_object();
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
				if (ImGui::MenuItem("Light component")) {
					if (object) {
						object->add_component<thebanana::light_component>();
					}
					else {
						thebanana::g_game->debug_print("could not add light component; no object selected");
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
			sprintf(buf, "%llu", object->get_uuid());
			ImGui::Text("Gameobject UUID: %s", buf);
			ImGui::Separator();
			ImGui::Text("Transform");
			glm::vec3 temp_translation = object->get_transform().get_translation();
			vec3_control("Translation", temp_translation);
			object->get_transform().set_translation(temp_translation);
			glm::vec3 temp_rotation = object->get_transform().get_rotation();
			for (int i = 0; i < 3; i++) temp_rotation[i] = glm::degrees(temp_rotation[i]);
			vec3_control("Rotation", temp_rotation, 0.f, 1.f);
			for (int i = 0; i < 3; i++) temp_rotation[i] = glm::radians(temp_rotation[i]);
			object->get_transform().set_rotation(temp_rotation);
			glm::vec3 temp_scale = object->get_transform().get_scale();
			vec3_control("Scale", temp_scale, 1.f);
			if (temp_scale.x < 0.0001f) temp_scale.x = 0.001f;
			if (temp_scale.y < 0.0001f) temp_scale.y = 0.001f;
			if (temp_scale.z < 0.0001f) temp_scale.z = 0.001f;
			object->get_transform().set_scale(temp_scale);
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
				else if (typeid(c).hash_code() == typeid(thebanana::light_component).hash_code()) label = "Light component";
				else if (typeid(c).hash_code() == typeid(thebanana::native_script_component).hash_code()) label = "Native script component";
				else if (typeid(c).hash_code() == typeid(thebanana::debug_component).hash_code()) label = "Debug component";
				if (!label) continue;
				char buf[256];
				sprintf(buf, "%ld", buf);
				std::string label_text = label + std::string(", UUID: ") + buf;
				if (ImGui::CollapsingHeader(label_text.c_str())) {
					ImGui::PushID(label_text.c_str());
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
					if (typeid(c).hash_code() == typeid(thebanana::rigidbody).hash_code()) {
						collider_control((thebanana::rigidbody&)c);
					}
					if (typeid(c).hash_code() == typeid(thebanana::light_component).hash_code()) {
						thebanana::light_component& lc = (thebanana::light_component&)c;
						thebanana::graphics::framebuffer* db = lc.get_depthbuffer();
						int texture = (int)(size_t)db->get_attachments()[db->get_attachment_map().depth_index].value;
						if (thebanana::graphics::util::is_2d(texture)) {
							if (ImGui::Button("View depth map")) {
								this->m_texture_viewer->set_texture(texture);
							}
						}
					}
					auto& properties = c.get_properties();
					for (size_t i = 0; i < properties.size(); i++) {
						auto it = properties.begin();
						std::advance(it, i);
						auto& p = *it;
						if (typeid(*p).hash_code() == typeid(thebanana::component::property<thebanana::gameobject*>).hash_code()) {
							thebanana::component::property<thebanana::gameobject*>* prop = (thebanana::component::property<thebanana::gameobject*>*)p.get();
							char buf[256];
							sprintf(buf, "%llu", c.get_uuid());
							std::string uuidstring = buf;
							sprintf(buf, "%llu", i);
							char id[256];
							sprintf(id, "%s, %s", uuidstring.c_str(), buf);
							ImGui::PushID(id);
							std::string text = (*prop->get_value()) ? (*prop->get_value())->get_nickname() : "None (Gameobject)";
							ImGui::InputText(prop->get_name().c_str(), &text, ImGuiInputTextFlags_ReadOnly);
							if (ImGui::BeginDragDropTarget()) {
								if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("GAMEOBJECT_PAYLOAD")) {
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
						} else if (typeid(*p).hash_code() == typeid(thebanana::component::property<thebanana::material*>).hash_code()) {
							thebanana::component::property<thebanana::material*>* prop = (thebanana::component::property<thebanana::material*>*)p.get();
							char buf[256];
							sprintf(buf, "%llu", c.get_uuid());
							std::string uuidstring = buf;
							sprintf(buf, "%llu", i);
							char id[256];
							sprintf(id, "%s, %s", uuidstring.c_str(), buf);
							ImGui::PushID(id);
							std::string text;
							if (*prop->get_value()) {
								text = (*prop->get_value())->get_friendly_name();
							} else {
								text = "None (Material)";
							}
							ImGui::InputText(prop->get_name().c_str(), &text, ImGuiInputTextFlags_ReadOnly);
							if (ImGui::BeginDragDropTarget()) {
								if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("MATERIAL_PAYLOAD")) {
									assert(payload->DataSize == sizeof(unsigned long long));
									unsigned long long uuid = *(unsigned long long*)payload->Data;
									*prop->get_value() = thebanana::g_game->get_material_registry()->find(uuid);
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
					ImGui::PopID();
				}
			}
		} else {
			ImGui::Text("There is no Gameobject selected.\nYou can select one from the scene's hierarchy panel.");
		}
		ImGui::End();
	}
	std::string property_editor_panel::get_menu_text() {
		return "Property Editor";
	}
	void property_editor_panel::set_hierarchy(scene_hierarchy_panel* hierarchy) {
		this->m_hierarchy = hierarchy;
	}
	void property_editor_panel::set_texture_viewer(texture_viewer_panel* viewer) {
		this->m_texture_viewer = viewer;
	}
}