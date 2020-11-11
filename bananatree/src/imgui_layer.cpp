#include <thebanana.h>
#include "imgui_layer.h"
#include <imgui.h>
#include <backends/imgui_impl_win32.h>
#include <backends/imgui_impl_opengl3.h>
#include <misc/cpp/imgui_stdlib.h>
#include "panels/viewport_panel.h"
namespace bananatree {
	imgui_layer::imgui_layer() {
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
		io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
		ImGui::StyleColorsDark();
		ImGuiStyle& style = ImGui::GetStyle();
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
			style.WindowRounding = 0.0f;
			style.Colors[ImGuiCol_WindowBg].w = 1.0f;
		}
		ImGui_ImplWin32_Init(thebanana::g_game->get_window(), wglGetCurrentContext());
		ImGui_ImplOpenGL3_Init("#version 460");
		this->add_panel(new viewport_panel);
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
	void imgui_layer::add_panel(panel* p) {
		this->m_panels.push_back(std::unique_ptr<panel>(p));
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
				if (ImGui::MenuItem("New", "Ctrl+N")) thebanana::g_game->get_scene()->clear();
				std::string scenefile = "../sandbox/scenes/test.basket";
				if (ImGui::MenuItem("Open...", "Ctrl+O")) {
					thebanana::scene_serializer serializer(thebanana::g_game->get_scene());
					serializer.deserialize(scenefile);
				}
				if (ImGui::MenuItem("Save As...", "Ctrl+Shift+S")) {
					thebanana::scene_serializer serializer(thebanana::g_game->get_scene());
					serializer.serialize(scenefile);
				}
				ImGui::Separator();
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
				ImGui::EndMenu();
			}
			ImGui::EndMenuBar();
		}
	}
}