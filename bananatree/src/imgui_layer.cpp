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
#include "panels/script_registry_panel.h"
#include "panels/material_editor_panel.h"
#include "panels/texture_viewer_panel.h"
#include "panels/lighting_panel.h"
#include "editor_layer.h"
#include "util.h"
namespace bananatree {
	void create_static_mesh(bool* open, thebanana::gameobject* parent) {
		std::string window_name = "Add static mesh to " + std::string(parent ? "gameobject" : "scene");
		ImGui::Begin(window_name.c_str(), open);
		static std::string mesh_name = "";
		ImGui::InputText("Mesh name", &mesh_name);
		if (ImGui::Button("Cancel")) {
			mesh_name = "";
			*open = false;
		}
		ImGui::SameLine();
		if (ImGui::Button("Confirm")) {
			thebanana::gameobject* object;
			if (parent) object = parent->add_object(new thebanana::static_mesh(mesh_name));
			else object = thebanana::g_game->get_scene()->add_object(new thebanana::static_mesh(mesh_name));
#ifndef BANANA_DEBUG
			object->add_component<thebanana::debug_component>();
#endif
			mesh_name = "";
			*open = false;
		}
		ImGui::End();
	}
	imgui_layer::imgui_layer(editor_layer* el) : m_editor_layer(el) {
		this->new_scene();
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
		ImGui_ImplWin32_Init((HWND)thebanana::g_game->get_window().m, wglGetCurrentContext());
		ImGui_ImplOpenGL3_Init("#version 460");
		this->add_panel<viewport_panel>()->set_imgui_layer(this);
		this->add_panel<log_panel>();
		this->add_panel<property_editor_panel>()->set_hierarchy(this->add_panel<scene_hierarchy_panel>());
		this->find_panel<property_editor_panel>()->set_texture_viewer(this->add_panel<texture_viewer_panel>());
		this->add_panel<model_registry_panel>()->set_project(this->m_editor_layer->get_project());
		this->add_panel<project_editor_panel>()->set_project(this->m_editor_layer->get_project());
		this->add_panel<script_registry_panel>();
		this->add_panel<material_editor_panel>()->set_project(this->m_editor_layer->get_project());
		this->find_panel<material_editor_panel>()->set_texture_viewer(this->find_panel<texture_viewer_panel>());
		this->add_panel<lighting_panel>();
	}
	imgui_layer::~imgui_layer() {
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplWin32_Shutdown();
		ImGui::DestroyContext();
	}
	void imgui_layer::update() {
		this->parse_inputs();
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
	void imgui_layer::new_scene() {
		this->m_temp_scene_path.clear();
		this->find_panel<scene_hierarchy_panel>()->set_selected_object(NULL);
		thebanana::g_game->get_scene()->clear();
		thebanana::gameobject* camera = new thebanana::basic_gameobject;
		camera->get_nickname() = "Camera";
		camera->get_transform().move(glm::vec3(0.f, 0.f, 2.f));
		camera->add_component<thebanana::camera_component>().set_property<bool>("Primary", true);
		thebanana::g_game->get_scene()->add_object(camera);
	}
	void imgui_layer::open_scene(const std::string& path) {
		this->find_panel<scene_hierarchy_panel>()->set_selected_object(NULL);
		thebanana::scene_serializer serializer(thebanana::g_game->get_scene());
		serializer.deserialize(path);
	}
	void imgui_layer::save_scene(const std::string& path) {
		thebanana::scene_serializer serializer(thebanana::g_game->get_scene());
		serializer.serialize(path);
	}
	void imgui_layer::open_scene() {
		std::string path = util::open_dialog("Banana Scene (*.basket)\0*.basket\0");
		if (!path.empty()) {
			this->m_temp_scene_path = path;
			this->open_scene(path);
		}
	}
	void imgui_layer::save_scene() {
		std::string path = util::save_dialog("Banana Scene (*.basket)\0*.basket\0");
		if (!path.empty()) {
			this->m_temp_scene_path = path;
			this->save_scene(path);
		}
	}
	void imgui_layer::save_scene_from_temp() {
		if (this->m_temp_scene_path.empty()) {
			this->save_scene();
		} else {
			this->save_scene(this->m_temp_scene_path);
		}
	}
	std::string imgui_layer::get_scene_path() {
		return this->m_temp_scene_path;
	}
	void imgui_layer::begin() {
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();
	}
	void imgui_layer::end() {
		ImGuiIO& io = ImGui::GetIO();
		glm::vec2 size = thebanana::g_game->get_window_size();
		io.DisplaySize = ImVec2(size.x, size.y);
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
					this->new_scene();
				}
				if (ImGui::MenuItem("Save Scene", "Ctrl+S")) {
					this->save_scene_from_temp();
				}
				if (ImGui::MenuItem("Save Scene As...", "Ctrl+Shift+S")) {
					this->save_scene();
				}
				if (ImGui::MenuItem("Open Scene...", "Ctrl+O")) {
					this->open_scene();
				}
				ImGui::Separator();
				if (ImGui::MenuItem("New Project")) {
					thebanana::g_game->get_model_registry()->reload(std::vector<thebanana::model_registry::model_descriptor>());
					this->m_editor_layer->get_project()->reset();
				}
				if (ImGui::MenuItem("Save Project")) {
					if (this->m_editor_layer->get_project()->has_temp_path()) {
						this->m_editor_layer->get_project()->save();
					} else {
						this->save_project_as();
					}
				}
				if (ImGui::MenuItem("Save Project As...")) {
					this->save_project_as();
				}
				if (ImGui::MenuItem("Open Project...")) {
					std::string path = util::open_dialog("Banana Project (*.tree)\0*.tree");
					if (!path.empty()) {
						thebanana::g_game->get_model_registry()->reload(std::vector<thebanana::model_registry::model_descriptor>());
						this->m_editor_layer->get_project()->load(path);
					}
				}
				ImGui::Separator();
				if (ImGui::MenuItem("Quit", "Alt+F4")) {
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
					thebanana::gameobject* object = thebanana::g_game->get_scene()->add_object(new thebanana::basic_gameobject);
#ifndef BANANA_DEBUG
					object->add_component<thebanana::debug_component>();
#endif
				}
				if (ImGui::MenuItem("Static mesh")) {
					this->m_static_mesh_creation_window_open = true;
				}
				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("Test")) {
				if (ImGui::MenuItem("Play")) {
					this->m_editor_layer->launch_sandbox();
				}
				if (ImGui::MenuItem("Compile")) {
					this->m_editor_layer->compile_scripts();
				}
				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("Gizmos")) {
				if (ImGui::BeginMenu("Transformation Gizmo...")) {
					if (ImGui::MenuItem("None", "Alt+W")) {
						this->find_panel<viewport_panel>()->set_gizmo_operation(viewport_panel::gizmo_operation::none);
					}
					if (ImGui::MenuItem("Translate", "Alt+E")) {
						this->find_panel<viewport_panel>()->set_gizmo_operation(viewport_panel::gizmo_operation::translate);
					}
					if (ImGui::MenuItem("Rotate", "Alt+R")) {
						this->find_panel<viewport_panel>()->set_gizmo_operation(viewport_panel::gizmo_operation::rotate);
					}
					if (ImGui::MenuItem("Scale", "Alt+T")) {
						this->find_panel<viewport_panel>()->set_gizmo_operation(viewport_panel::gizmo_operation::scale);
					}
					ImGui::EndMenu();
				}
				if (ImGui::BeginMenu("Transformation mode...")) {
					if (ImGui::MenuItem("Local")) {
						this->find_panel<viewport_panel>()->set_transformation_mode(viewport_panel::transformation_mode::local);
					}
					if (ImGui::MenuItem("World")) {
						this->find_panel<viewport_panel>()->set_transformation_mode(viewport_panel::transformation_mode::world);
					}
					ImGui::EndMenu();
				}
				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("Shaders")) {
				if (ImGui::MenuItem("Material preview (Default)")) {
					thebanana::g_game->get_scene()->set_shader_name("material preview");
				}
				if (ImGui::MenuItem("Rendered")) {
					thebanana::g_game->get_scene()->set_shader_name("rendered");
				}
				ImGui::EndMenu();
			}
#ifdef BANANA_DEBUG
			if (ImGui::BeginMenu("Debug")) {
				if (ImGui::MenuItem("Break")) {
					__debugbreak();
				}
				ImGui::EndMenu();
			}
#endif
			ImGui::EndMenuBar();
			if (this->m_static_mesh_creation_window_open) create_static_mesh(&this->m_static_mesh_creation_window_open, NULL);
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
	void imgui_layer::parse_inputs() {
		auto input_manager = thebanana::g_game->get_input_manager();
		bool control = input_manager->get_key(thebanana::key_ctrl).held;
		bool shift = input_manager->get_key(thebanana::key_shift).held;
		bool alt = input_manager->get_key(thebanana::key_alt).held;
		if (input_manager->get_key(thebanana::key_n).down && control) {
			this->new_scene();
		}
		if (input_manager->get_key(thebanana::key_o).down && control) {
			this->open_scene();
		}
		if (input_manager->get_key(thebanana::key_s).down) {
			if (control && shift) this->save_scene();
			else if (control) this->save_scene_from_temp();
		}
		if (input_manager->get_key(thebanana::key_w).down && alt) {
			this->find_panel<viewport_panel>()->set_gizmo_operation(viewport_panel::gizmo_operation::none);
		}
		if (input_manager->get_key(thebanana::key_e).down && alt) {
			this->find_panel<viewport_panel>()->set_gizmo_operation(viewport_panel::gizmo_operation::translate);
		}
		if (input_manager->get_key(thebanana::key_r).down && alt) {
			this->find_panel<viewport_panel>()->set_gizmo_operation(viewport_panel::gizmo_operation::rotate);
		}
		if (input_manager->get_key(thebanana::key_t).down && alt) {
			this->find_panel<viewport_panel>()->set_gizmo_operation(viewport_panel::gizmo_operation::scale);
		}
	}
	void imgui_layer::save_project_as() {
		std::string path = util::save_dialog("Banana Project (*.tree)\0*.tree");
		if (!path.empty()) {
			this->m_editor_layer->get_project()->save(path);
		}
	}
}