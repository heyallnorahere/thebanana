#include "pch.h"
#include "debug_tools.h"
#include "game.h"
#include "scene.h"
#include "gameobject.h"
#include "input_manager.h"
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
void tree(gameobject* parent);
void tree_helper(gameobject* object) {
	if (object->get_children_count() > 0) {
		tree(object);
	}
	else {
		ImGui::Indent(ImGui::GetTreeNodeToLabelSpacing());
		ImGui::Text("object, no children");
		ImGui::Unindent();
	}
}
void tree(gameobject* parent) {
	char buf[256];
	sprintf(buf, "object, children: %d", parent->get_children_count());
	if (ImGui::TreeNodeEx(buf, open_flags)) {
		for (size_t i = 0; i < parent->get_children_count(); i++) {
			gameobject* obj = parent->get_child(i);
			tree_helper(obj);
		}
		ImGui::TreePop();
	}
}
void scene_hierarchy(scene* scene) {
	ImGui::Begin("scene hierarchy");
	if (ImGui::TreeNodeEx("root", ImGuiTreeNodeFlags_Leaf)) {
		for (size_t i = 0; i < scene->get_children_count(); i++) {
			gameobject* obj = scene->get_child(i);
			tree_helper(obj);
		}
		ImGui::TreePop();
	}
	ImGui::End();
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
	ImGui::End();
}
void render_imgui(game* g_game) {
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
	scene_hierarchy(g_game->get_scene());
	debug_menu(g_game);
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}
