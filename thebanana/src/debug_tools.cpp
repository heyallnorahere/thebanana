#include "pch.h"
#include "debug_tools.h"
#include "game.h"
#include "scene.h"
#include "gameobject.h"
#include "input_manager.h"
#include "physics/rigidbody.h"
#include "graphics/framebuffer.h"
#include "ui/ui.h"
namespace thebanana {
	namespace debug {
		std::stringstream debug_log;
		void init_imgui(HWND window) {
			IMGUI_CHECKVERSION();
			ImGui::CreateContext();
			ImGuiIO& io = ImGui::GetIO(); (void)io;
			ImGui::StyleColorsDark();
			ImGui_ImplWin32_Init(window);
			ImGui_ImplOpenGL3_Init("#version 460");
		}
		void clean_up_imgui() {
			ImGui_ImplOpenGL3_Shutdown();
			ImGui_ImplWin32_Shutdown();
			ImGui::DestroyContext();
		}
		constexpr ImGuiTreeNodeFlags open_flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick;
		static int id;
#define ID() char id_str[256]; sprintf(id_str, "node%d", id++)
		gameobject* current_selected_gameobject = NULL;
		static void set_current_gameobject(gameobject* obj, gameobject** object_ptr) {
			if (ImGui::IsItemClicked()) {
				*object_ptr = obj;
			}
		}
		void tree(gameobject* parent, gameobject** object_ptr);
		void tree_helper(gameobject* object, gameobject** object_ptr) {
			if (object->get_children_count() > 0) {
				tree(object, object_ptr);
			}
			else {
				ID();
				if (ImGui::TreeNodeEx(id_str, ImGuiTreeNodeFlags_Leaf, "%s, no children", object->get_nickname().c_str())) {
					set_current_gameobject(object, object_ptr);
					ImGui::TreePop();
				}
			}
		}
		void tree(gameobject* parent, gameobject** object_ptr) {
			ID();
			if (ImGui::TreeNodeEx(id_str, open_flags, "%s, children: %d", parent->get_nickname().c_str(), parent->get_children_count())) {
				set_current_gameobject(parent, object_ptr);
				for (size_t i = 0; i < parent->get_children_count(); i++) {
					gameobject* obj = parent->get_child(i);
					tree_helper(obj, object_ptr);
				}
				ImGui::TreePop();
			}
		}
		bool scene_hierarchy(scene* scene, bool is_selection_window = false, gameobject** object_ptr = &current_selected_gameobject, int id = 0) {
			::thebanana::debug::id = 0xff;
			std::string window_name = "the banana: scene hierarchy";
			if (is_selection_window) {
				std::stringstream ss;
				ss << "the banana: selection window #" << id;
				window_name = ss.str();
			}
			ImGui::Begin(window_name.c_str());
			if (is_selection_window) {
				bool confirmed = ImGui::Button("confirm");
				ImGui::SameLine();
				bool closed = confirmed || ImGui::Button("close");
				if (closed) {
					if (!confirmed) {
						*object_ptr = NULL;
					}
					ImGui::End();
					return true;
				}
			}
			if (ImGui::TreeNodeEx("root", ImGuiTreeNodeFlags_Leaf)) {
				set_current_gameobject(NULL, object_ptr);
				for (size_t i = 0; i < scene->get_children_count(); i++) {
					gameobject* obj = scene->get_child(i);
					tree_helper(obj, object_ptr);
				}
				ImGui::TreePop();
			}
			ImGui::End();
			return false;
		}
		bool control = true;
		void debug_menu(game* g_game) {
			ImGui::Begin("the banana: debug menu");
			ImGui::TextColored(ImVec4(1.f, 0.f, 1.f, 1.f), "input");
			ImGui::Checkbox("controls active", &control);
			ImGui::Checkbox("cursor visible", &g_game->showing_cursor());
			if (ImGui::TreeNodeEx("devices", open_flags)) {
				ImGui::Indent(ImGui::GetTreeNodeToLabelSpacing());
				for (size_t i = 0; i < g_game->get_input_manager()->get_num_devices(); i++) {
					std::string type;
					switch (g_game->get_input_manager()->get_device_type(i)) {
					case input_manager::device_type::keyboard:
						type = "keyboard";
						break;
					case input_manager::device_type::mouse:
						type = "mouse";
						break;
					case input_manager::device_type::controller:
						type = "controller";
						break;
					default:
						type = "other";
						break;
					}
					std::string text = type + ", id: %d";
					ImGui::Text(text.c_str(), i);
				}
				ImGui::Unindent();
				ImGui::TreePop();
			}
			ImGui::TextColored(ImVec4(1.f, 1.f, 0.f, 1.f), "physics");
			ImGui::InputFloat3("gravity", &rigidbody::gravity.x);
			ImGui::End();
		}
		struct selection_struct {
			component::property_base* prop;
			gameobject** ptr;
		};
		std::map<gameobject*, bool> transform_menu_values;
		void property_editor(game* g_game, std::vector<selection_struct>& ptrs) {
			ImGui::Begin("the banana: property editor");
			if (current_selected_gameobject) {
				char buf[256];
				_ui64toa(reinterpret_cast<size_t>(current_selected_gameobject), buf, 0x10);
				ImGui::Text("gameobject address: 0x%s", buf);
				ImGui::InputText("gameobject nickname", &current_selected_gameobject->get_nickname());
				if (ImGui::Button("toggle transform menu")) {
					transform_menu_values[current_selected_gameobject] = !(transform_menu_values[current_selected_gameobject]);
				}
				const component::properties_t& p = current_selected_gameobject->get_properties();
				for (auto& pr : p) {
					pr->draw();
					if (pr->is_selection_window_open()) {
						gameobject** ptr = pr->get_selection_window_ptr();
						if (ptr) {
							ptrs.push_back({ pr.get(), ptr });
						}
					}
				}
				for (size_t i = 0; i < current_selected_gameobject->get_number_components<component>(); i++) {
					component& c = current_selected_gameobject->get_component<component>(i);
					ImGui::Text("component: %s", typeid(c).name());
					const component::properties_t& properties = c.get_properties();
					for (auto& p : properties) {
						p->draw();
						if (p->is_selection_window_open()) {
							gameobject** ptr = p->get_selection_window_ptr();
							if (ptr) {
								ptrs.push_back({ p.get(), ptr });
							}
						}
					}
				}
			}
			else {
				ImGui::Text("there is no gameobject selected");
			}
			ImGui::End();
		}
		void debug_console() {
			ImGui::Begin("the banana: console");
			std::string log = debug_log.str();
			ImGui::InputTextMultiline("console log", &log, ImVec2(1000, 1000), ImGuiInputTextFlags_ReadOnly);
			ImGui::BeginChild("console log");
			ImGui::SetScrollY(ImGui::GetScrollMaxY());
			ImGui::EndChild();
			ImGui::End();
		}
		void transform_menu(gameobject* obj, int id) {
			std::stringstream ss;
			ss << "the banana: transform menu #" << id;
			std::string window_name = ss.str();
			ImGui::Begin(window_name.c_str());
			ImGui::Text("transform menu for gameobject: %s", obj->get_nickname().c_str());
			static glm::vec3 translate(0.f);
			ImGui::InputFloat3("translation values", &translate.x);
			if (ImGui::Button("translate")) {
				obj->get_transform().translate(translate);
				translate = glm::vec3(0.f);
			}
			static glm::vec3 move(0.f);
			ImGui::InputFloat3("motion values", &move.x);
			if (ImGui::Button("move")) {
				obj->get_transform().move(move);
				move = glm::vec3(0.f);
			}
			static glm::vec3 rotate(0.f);
			static bool rotate_x = false, rotate_y = false, rotate_z = false;
			ImGui::Checkbox("rotate x", &rotate_x);
			ImGui::Checkbox("rotate y", &rotate_y);
			ImGui::Checkbox("rotate z", &rotate_z);
			ImGui::InputFloat3("rotation values", &rotate.x);
			if (ImGui::Button("rotate")) {
				if (rotate_x) {
					obj->get_transform().rotate(rotate.x, glm::vec3(1.f, 0.f, 0.f));
					rotate_x = false;
				}
				if (rotate_y) {
					obj->get_transform().rotate(rotate.y, glm::vec3(0.f, 1.f, 0.f));
					rotate_y = false;
				}
				if (rotate_z) {
					obj->get_transform().rotate(rotate.z, glm::vec3(0.f, 0.f, 1.f));
					rotate_z = false;
				}
				rotate = glm::vec3(0.f);
			}
			static glm::vec3 scale(1.f);
			ImGui::InputFloat3("dilation values", &scale.x);
			if (ImGui::Button("scale")) {
				obj->get_transform().scale(scale);
				scale = glm::vec3(1.f);
			}
			if (ImGui::Button("close")) {
				transform_menu_values[obj] = false;
			}
			ImGui::End();
		}
		void render_imgui(game* g_game) {
			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplWin32_NewFrame();
			ImGui::NewFrame();
			scene_hierarchy(g_game->get_scene());
			debug_menu(g_game);
			std::vector<selection_struct> ptrs;
			property_editor(g_game, ptrs);
			for (int i = 0; i < ptrs.size(); i++) {
				auto p = ptrs[i];
				if (scene_hierarchy(g_game->get_scene(), true, p.ptr, i + 1)) {
					p.prop->close_selection_window();
				}
			}
			std::vector<gameobject*> objects;
			for (auto p : transform_menu_values) {
				if (p.second) objects.push_back(p.first);
			}
			for (int i = 0; i < objects.size(); i++) {
				transform_menu(objects[i], i + 1);
			}
			debug_console();
			ImGui::Render();
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		}
		void log_print(const std::string& message) {
			debug_log << message << "\n";
		}
	}
}