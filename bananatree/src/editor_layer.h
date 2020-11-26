#pragma once
#include "imgui_layer.h"
#include "project.h"
#include "editorconfig.h"
namespace bananatree {
	class editor_layer : public thebanana::application_layer {
	public:
		virtual void init() override;
		virtual void gameloop() override;
		virtual std::string window_title() override;
		std::shared_ptr<project> get_project();
		std::shared_ptr<imgui_layer> get_imgui_layer();
		void launch_sandbox();
		void compile_scripts();
		void attach_scripts();
	private:
		std::shared_ptr<imgui_layer> m_imgui_layer;
		std::shared_ptr<project> m_project;
		std::shared_ptr<editorconfig> m_config;
		std::string get_dll_path();
	};
}