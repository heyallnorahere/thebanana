#pragma once
#include "includelist.h"
#include "components/components.h"
#include "graphics/graphics.h"
#include "physics/physics.h"
#include "particlesystem/particlesystem.h"
#include "main_module.h"
#ifdef BANANA_MAIN
::thebanana::application_layer* create_application_layer();
int __stdcall WinMain(HINSTANCE instance, HINSTANCE prev, char* cmd_line, int cmd_show) {
	::thebanana::application_layer* app_layer = create_application_layer();
	::thebanana::window::setup_window();
	::thebanana::g_game = new ::thebanana::game(app_layer->window_title(), ::thebanana::get_current_module());
	app_layer->register_scripts();
	app_layer->init();
	int return_code = ::thebanana::window::window_loop(app_layer);
	app_layer->clean_up();
	delete app_layer;
	delete ::thebanana::g_game;
	return return_code;
}
#endif//BANANA_MAIN