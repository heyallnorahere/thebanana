#include <thebanana.h>
#include "editor_layer.h"
#include "util.h"
#include "panels/script_registry_panel.h"
#include "panels/scene_hierarchy_panel.h"
namespace bananatree {
	void editor_layer::init() {
		this->m_project = std::shared_ptr<project>(new project);
		this->m_imgui_layer = new imgui_layer(this);
		this->m_project->set_editor_layer(this);
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
		thebanana::g_game->get_shader_registry()->register_shader("rendered", "shaders/rendered/vertex.shader", "shaders/rendered/fragment.shader");
		thebanana::g_game->get_shader_registry()->register_shader("material preview", "shaders/material_preview/vertex.shader", "shaders/material_preview/fragment.shader");
		thebanana::g_game->get_shader_registry()->register_shader("depth", "shaders/depth/vertex.shader", "shaders/depth/fragment.shader");
		thebanana::g_game->get_shader_registry()->register_shader("point_depth", "shaders/point_depth/vertex.shader", "shaders/point_depth/fragment.shader", "shaders/point_depth/geometry.shader");
		thebanana::g_game->get_scene()->set_shader_name("material preview");
		thebanana::g_game->get_scene()->set_depth_shader_name(thebanana::light_component::light_type::point, "point_depth");
		thebanana::g_game->get_scene()->set_depth_shader_name(thebanana::light_component::light_type::directional, "depth");
		thebanana::g_game->get_scene()->set_depth_shader_name(thebanana::light_component::light_type::spotlight, "depth");
		std::string path = this->m_config->get<std::string>("startupproject");
		if (!path.empty()) {
			this->m_project->load(path);
		}
		path = this->m_config->get<std::string>("startupscene");
		if (!path.empty()) {
			this->m_imgui_layer->open_scene(path);
		}
	}
	void editor_layer::gameloop() {
		thebanana::g_game->update();
		this->m_imgui_layer->update();
		thebanana::g_game->clear_screen();
		this->m_imgui_layer->render();
		thebanana::g_game->swap_buffers();
	}
	void editor_layer::clean_up() {
		delete this->m_imgui_layer;
	}
	std::string editor_layer::window_title() {
		return "The Banana Tree";
	}
	std::shared_ptr<project> editor_layer::get_project() {
		return this->m_project;
	}
	imgui_layer* editor_layer::get_imgui_layer() {
		return this->m_imgui_layer;
	}
	void editor_layer::launch_sandbox() {
		PROCESS_INFORMATION pi = { 0 };
		STARTUPINFOA si = { 0 };
		std::string config =
#ifdef BANANA_DEBUG
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
		std::string code_project_path = this->m_project->get_code_project_path();
		if (!code_project_path.empty()) {
			args += " \"" + this->get_dll_path() + "\"";
		}
		std::string workdir = this->m_config->get<std::string>("sandboxworkdir");
		thebanana::g_game->debug_print("launching sandbox with command: " + args + " in " + workdir);
		CreateProcessA(path.c_str(), (char*)args.c_str(), NULL, NULL, false, 0, NULL, workdir.c_str(), &si, &pi);
	}
	void editor_layer::compile_scripts() {
		std::string temp_scene_path = util::make_temp_path("bananatree_temp_scene");
		thebanana::scene_serializer serializer(thebanana::g_game->get_scene());
		serializer.serialize(temp_scene_path);
		thebanana::g_game->get_scene()->clear();
		thebanana::g_game->unload_script_module();
		std::string code_project_path = this->m_project->get_code_project_path();
		if (code_project_path.empty()) {
			return;
		}
		PROCESS_INFORMATION pi = { 0 };
		STARTUPINFOA si = { 0 };
		std::string msbuild_path = this->m_config->get<std::string>("msbuild");
		std::string config =
#ifdef BANANA_DEBUG
			"Debug";
#else
			"Release";
#endif
		std::string command = "msbuild /property:Configuration=" + config + " .";
		CreateProcessA(msbuild_path.c_str(), (char*)command.c_str(), NULL, NULL, NULL, NULL, NULL, code_project_path.c_str(), &si, &pi);
		WaitForSingleObject(pi.hProcess, INFINITE);
		this->attach_scripts();
		this->m_imgui_layer->find_panel<scene_hierarchy_panel>()->set_selected_object(NULL);
		serializer.deserialize(temp_scene_path);
	}
	void editor_layer::attach_scripts() {
		std::string code_project_path = this->m_project->get_code_project_path();
		if (code_project_path.empty()) {
			return;
		}
		thebanana::g_game->load_script_module(this->get_dll_path());
		this->m_imgui_layer->find_panel<script_registry_panel>()->refresh();
	}
	std::string editor_layer::get_dll_path() {
		std::string project_path = this->m_project->get_code_project_path();
		std::string configuration =
#ifdef BANANA_DEBUG
			"Debug";
#else
			"Release";
#endif
		std::string directory = project_path + (project_path == "." ? "\\" : "") + "x64\\" + configuration + "\\";
		std::string path = directory + this->m_project->get_dll_name();
		return path;
	}
}