#include <thebanana.h>
#include "imgui_layer.h"
#include <imgui.h>
#include <backends/imgui_impl_win32.h>
#include <backends/imgui_impl_opengl3.h>
#include <misc/cpp/imgui_stdlib.h>
#include "panels/viewport_panel.h"
#include "panels/log_panel.h"
#include "panels/scene_hierarchy_panel.h"
#include "panels/property_editor_panel.h"
#include "panels/model_registry_panel.h"
#include "panels/project_editor_panel.h"
#include "editor_layer.h"
#include "util.h"
#include "../resource.h"
namespace bananatree {
	static void create_static_mesh(bool* open) {
		ImGui::Begin("Create static mesh", open);
		static std::string mesh_name = "";
		ImGui::InputText("Mesh name", &mesh_name);
		if (ImGui::Button("Cancel")) {
			mesh_name = "";
			*open = false;
		}
		ImGui::SameLine();
		if (ImGui::Button("Confirm")) {
			thebanana::g_game->get_scene()->add_object(new thebanana::static_mesh(mesh_name));
			mesh_name = "";
			*open = false;
		}
		ImGui::End();
	}
	imgui_layer::imgui_layer(editor_layer* el) : m_editor_layer(el) {
		this->m_static_mesh_creation_window_open = false;
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
		io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
		io.Fonts->AddFontFromFileTTF("fonts/OpenSans-Bold.ttf", 18.f);
		io.FontDefault = io.Fonts->AddFontFromFileTTF("fonts/OpenSans-Regular.ttf", 18.f);
		ImGui::StyleColorsDark();
		ImGuiStyle& style = ImGui::GetStyle();
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
			style.WindowRounding = 0.0f;
			style.Colors[ImGuiCol_WindowBg].w = 1.0f;
		}
		this->set_theme();
		ImGui_ImplWin32_Init(thebanana::g_game->get_window(), wglGetCurrentContext());
		ImGui_ImplOpenGL3_Init("#version 460");
		this->add_panel<viewport_panel>();
		this->add_panel<log_panel>();
		this->add_panel<property_editor_panel>()->set_hierarchy(this->add_panel<scene_hierarchy_panel>());
		this->add_panel<model_registry_panel>();
		this->add_panel<project_editor_panel>()->set_project(this->m_editor_layer->get_project());
	}
	imgui_layer::~imgui_layer() {
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplWin32_Shutdown();
		ImGui::DestroyContext();
	}
	void imgui_layer::update() {
		for (auto& p : this->m_panels) {
			p->update();
		}
	}
	void imgui_layer::render() {
		this->begin();
		this->start_dockspace();
		for (auto& p : this->m_panels) {
			if (p->is_open()) p->render();
		}
		ImGui::End();
		this->end();
	}
	panel* imgui_layer::get_panel(size_t index) {
		auto& p = this->m_panels[index];
		return p.get();
	}
	size_t imgui_layer::get_panel_count() const {
		return this->m_panels.size();
	}
	void imgui_layer::begin() {
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();
	}
	void imgui_layer::end() {
		ImGuiIO& io = ImGui::GetIO();
		RECT r;
		GetWindowRect(thebanana::g_game->get_window(), &r);
		float width = static_cast<float>(r.right - r.left);
		float height = static_cast<float>(r.bottom - r.top);
		io.DisplaySize = ImVec2(width, height);
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
			thebanana::g_game->make_context_current();
		}
	}
	void imgui_layer::start_dockspace() {
		static bool dockspace_open = true;
		static bool opt_fullscreen_persistant = true;
		bool opt_fullscreen = opt_fullscreen_persistant;
		static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;
		ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
		if (opt_fullscreen) {
			ImGuiViewport* viewport = ImGui::GetMainViewport();
			ImGui::SetNextWindowPos(viewport->Pos);
			ImGui::SetNextWindowSize(viewport->Size);
			ImGui::SetNextWindowViewport(viewport->ID);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.f);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.f);
			window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
			window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
		}
		if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode) {
			window_flags |= ImGuiWindowFlags_NoBackground;
		}
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.f, 0.f));
		ImGui::Begin("banana tree editor", &dockspace_open, window_flags);
		ImGui::PopStyleVar();
		if (opt_fullscreen) ImGui::PopStyleVar(2);
		ImGuiIO& io = ImGui::GetIO();
		ImGuiStyle& style = ImGui::GetStyle();
		float min_win_size_x = style.WindowMinSize.x;
		style.WindowMinSize.x = 370.f;
		if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable) {
			ImGuiID dockspace_id = ImGui::GetID("bananatreedockspace");
			ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
		}
		style.WindowMinSize.x = min_win_size_x;
		if (ImGui::BeginMenuBar()) {
			if (ImGui::BeginMenu("File")) {
				if (ImGui::MenuItem("New Scene", "Ctrl+N")) {
					this->find_panel<scene_hierarchy_panel>()->set_selected_object(NULL);
					thebanana::g_game->get_scene()->clear();
				}
				if (ImGui::MenuItem("Open Scene...", "Ctrl+O")) {
					std::string path = open_dialog("Banana Scene (*.basket)\0*.basket\0");
					if (!path.empty()) {
						this->find_panel<scene_hierarchy_panel>()->set_selected_object(NULL);
						thebanana::scene_serializer serializer(thebanana::g_game->get_scene());
						serializer.deserialize(path);
					}
				}
				if (ImGui::MenuItem("Save Scene As...", "Ctrl+Shift+S")) {
					std::string path = save_dialog("Banana Scene (*.basket)\0*.basket\0");
					if (!path.empty()) {
						thebanana::scene_serializer serializer(thebanana::g_game->get_scene());
						serializer.serialize(path);
					}
				}
				ImGui::Separator();
				if (ImGui::MenuItem("New Project")) {
					this->m_editor_layer->get_project()->reset();
				}
				if (ImGui::MenuItem("Open Project...")) {
					std::string path = open_dialog("Banana Project (*.tree)\0*.tree");
					if (!path.empty()) {
						this->m_editor_layer->get_project()->load(path);
					}
				}
				if (ImGui::MenuItem("Save Project As...")) {
					std::string path = save_dialog("Banana Project (*.tree)\0*.tree");
					if (!path.empty()) {
						this->m_editor_layer->get_project()->save(path);
					}
				}
				if (ImGui::MenuItem("Quit", "Ctrl+Q")) {
					thebanana::g_game->destroy();
				}
				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("View")) {
				for (auto& p : this->m_panels) {
					ImGui::MenuItem(p->get_menu_text().c_str(), NULL, &p->is_open());
				}
				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("Gameobject")) {
				if (ImGui::MenuItem("Empty")) {
					thebanana::g_game->get_scene()->add_object(new thebanana::basic_gameobject);
				}
				if (ImGui::MenuItem("Static mesh")) {
					this->m_static_mesh_creation_window_open = true;
				}
				ImGui::EndMenu();
			}
#ifdef _DEBUG
			if (ImGui::BeginMenu("Debug")) {
				if (ImGui::MenuItem("Break")) {
					__debugbreak();
				}
				ImGui::EndMenu();
			}
#endif
			ImGui::EndMenuBar();
			if (this->m_static_mesh_creation_window_open) create_static_mesh(&this->m_static_mesh_creation_window_open);
		}
	}
	//https://github.com/TheCherno/Hazel/blob/master/Hazel/src/Hazel/ImGui/ImGuiLayer.cpp
	void imgui_layer::set_theme() {
		auto& colors = ImGui::GetStyle().Colors;
		colors[ImGuiCol_WindowBg] = ImVec4(0.1f, 0.105f, 0.11f, 1.f);
		colors[ImGuiCol_Header] = ImVec4(0.2f, 0.205f, 0.21f, 1.f);
		colors[ImGuiCol_HeaderHovered] = ImVec4(0.3f, 0.305f, 0.31f, 1.f);
		colors[ImGuiCol_HeaderActive] = ImVec4(0.15f, 0.1505f, 0.151f, 1.f);
		colors[ImGuiCol_Button] = ImVec4(0.2f, 0.205f, 0.21f, 1.f);
		colors[ImGuiCol_ButtonHovered] = ImVec4(0.3f, 0.305f, 0.31f, 1.f);
		colors[ImGuiCol_ButtonActive] = ImVec4(0.15f, 0.1505f, 0.151f, 1.f);
		colors[ImGuiCol_FrameBg] = ImVec4(0.2f, 0.205f, 0.21f, 1.f);
		colors[ImGuiCol_FrameBgHovered] = ImVec4(0.3f, 0.305f, 0.31f, 1.f);
		colors[ImGuiCol_FrameBgActive] = ImVec4(0.15f, 0.1505f, 0.151f, 1.f);
		colors[ImGuiCol_Tab] = ImVec4(0.15f, 0.1505f, 0.151f, 1.f);
		colors[ImGuiCol_TabHovered] = ImVec4(0.38f, 0.3805f, 0.381f, 1.f);
		colors[ImGuiCol_TabActive] = ImVec4(0.28f, 0.2805f, 0.281f, 1.f);
		colors[ImGuiCol_TabUnfocused] = ImVec4(0.15f, 0.1505f, 0.151f, 1.f);
		colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.2f, 0.205f, 0.21f, 1.f);
		colors[ImGuiCol_TitleBg] = ImVec4(0.15f, 0.1505f, 0.151f, 1.f);
		colors[ImGuiCol_TitleBgActive] = ImVec4(0.15f, 0.1505f, 0.151f, 1.f);
		colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.15f, 0.1505f, 0.151f, 1.f);
	}
}