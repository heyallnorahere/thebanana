#include "pch.h"
#include "debug_tools.h"
#include "game.h"
#include "scene.h"
#include "gameobject.h"
#include "input_manager.h"
#include "camera.h"
#include "physics/rigidbody.h"
#include "graphics/framebuffer.h"
namespace thebanana {
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
		::thebanana::id = 0xff;
		std::string window_name = "scene hierarchy";
		if (is_selection_window) {
			std::stringstream ss;
			ss << "selection window #" << id;
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
		ImGui::Begin("debug menu");
		ImGui::TextColored(ImVec4(1.f, 0.f, 1.f, 1.f), "input");
		ImGui::Checkbox("controls active", &control);
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
	void property_editor(game* g_game, std::vector<selection_struct>& ptrs) {
		ImGui::Begin("property editor");
		if (current_selected_gameobject) {
			char buf[256];
			_ui64toa(reinterpret_cast<size_t>(current_selected_gameobject), buf, 0x10);
			ImGui::Text("gameobject address: 0x%s", buf);
			ImGui::InputText("gameobject nickname", &current_selected_gameobject->get_nickname());
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
		ImGui::Begin("console");
		std::string log = debug_log.str();
		ImGui::InputTextMultiline("console log", &log, ImVec2(1000, 1000), ImGuiInputTextFlags_ReadOnly);
		ImGui::BeginChild("console log");
		ImGui::SetScrollY(ImGui::GetScrollMaxY());
		ImGui::EndChild();
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
		debug_console();
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}
	void log_print(const std::string& message) {
		debug_log << message << "\n";
	}
}