#include <thebanana.h>
#include "editor_layer.h"
#include "../resource.h"
#include "util.h"
namespace bananatree {
	void editor_layer::init() {
		this->m_project = std::shared_ptr<project>(new project);
		this->m_imgui_layer = std::shared_ptr<imgui_layer>(new imgui_layer(this));
		this->m_project->set_imgui_layer(&(*this->m_imgui_layer));
		this->m_config = std::shared_ptr<editorconfig>(new editorconfig);
		std::string config_filename = "bananatreeconfig.yaml";
		if (util::file_exists(config_filename)) {
			this->m_config->load(config_filename);
		} else {
			this->m_config->restore_defaults();
			this->m_config->save(config_filename);
		}
		thebanana::g_game->show_cursor();
		thebanana::g_game->unclip_cursor();
		thebanana::g_game->get_shader_registry()->register_shader("basic", new opengl_shader_library::win32_resource_shader(IDR_BASIC_VERTEX, IDR_BASIC_FRAGMENT));
		thebanana::g_game->get_scene()->set_shader_name("basic");
	}
	void editor_layer::gameloop() {
		thebanana::g_game->update();
		this->m_imgui_layer->update();
		thebanana::g_game->clear_screen();
		this->m_imgui_layer->render();
		thebanana::g_game->swap_buffers();
	}
	std::string editor_layer::window_title() {
		return "The Banana Tree";
	}
	std::shared_ptr<project> editor_layer::get_project() {
		return this->m_project;
	}
	void editor_layer::launch_sandbox() {
		PROCESS_INFORMATION pi = { 0 };
		STARTUPINFOA si = { 0 };
		std::string config =
#ifdef _DEBUG
			"Debug";
#else
			"Release";
#endif
		std::string config_string = "$(Configuration)";
		std::string path = this->m_config->get<std::string>("sandboxexe");
		size_t pos = path.find(config_string);
		if (pos != std::string::npos) {
			path = path.replace(pos, config_string.length(), config);
		}
		std::string args =
			"\"" + path + "\" \"" + this->m_imgui_layer->get_scene_path() + "\" \"" + this->m_project->get_path() + "\"";
		thebanana::debug::log_print("launching sandbox with command: " + args);
		CreateProcessA(path.c_str(), (char*)args.c_str(), NULL, NULL, false, 0, NULL, NULL, &si, &pi);
	}
	void editor_layer::compile_scripts() {
		std::string code_project_path = this->m_project->get_code_project_path();
		if (code_project_path.empty()) {
			return;
		}
		PROCESS_INFORMATION pi = { 0 };
		STARTUPINFOA si = { 0 };
		std::string msbuild_path = this->m_config->get<std::string>("msbuild");
		std::string command = "msbuild .";
		CreateProcessA(msbuild_path.c_str(), (char*)command.c_str(), NULL, NULL, NULL, NULL, NULL, code_project_path.c_str(), &si, &pi);
		WaitForSingleObject(pi.hProcess, INFINITE);
		this->attach_scripts();
	}
	void editor_layer::attach_scripts() {
		std::string project_path = this->m_project->get_code_project_path();
		std::string directory = project_path + (project_path == "." ? "\\" : "") + "x64\\Debug\\";
		std::string path = directory + "*.dll";
		WIN32_FIND_DATAA wfd;
		ZeroMemory(&wfd, sizeof(WIN32_FIND_DATAA));
		HANDLE h = FindFirstFileA(path.c_str(), &wfd);
		assert(h != INVALID_HANDLE_VALUE);
		do {
			thebanana::g_game->load_script_module(directory + wfd.cFileName);
			break;
		} while (FindNextFileA(h, &wfd));
	}
}