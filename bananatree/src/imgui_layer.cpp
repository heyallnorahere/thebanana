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
		ImGui::StyleColorsDark();
		ImGui_ImplWin32_Init(thebanana::g_game->get_window());
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
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}
}