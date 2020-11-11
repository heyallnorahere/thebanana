#include <thebanana.h>
#include "imgui_layer.h"
#include <imgui.h>
#include <backends/imgui_impl_win32.h>
#include <backends/imgui_impl_opengl3.h>
#include <misc/cpp/imgui_stdlib.h>
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
		for (auto& p : this->m_panels) {
			p->render();
		}
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
}